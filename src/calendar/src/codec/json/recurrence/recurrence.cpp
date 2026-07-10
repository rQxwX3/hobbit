#include <codec/iso8601/datetime.hpp>
#include <codec/iso8601/opt_datetime.hpp>
#include <codec/json/recurrence/interval_pattern.hpp>
#include <codec/json/recurrence/null_pattern.hpp>
#include <codec/json/recurrence/recurrence.hpp>
#include <codec/json/recurrence/weekdays_pattern.hpp>
#include <error.hpp>
#include <json.hpp>
#include <recurrence/recurrence.hpp>

namespace clndr::codec::json::rec {
[[nodiscard]] auto Recurrence::encode(const clndr::rec::Recurrence &recurrence)
    -> nlohmann::json {
    auto startDateTimeJSON{
        codec::iso8601::DateTime::encode(recurrence.getStartDateTime())};
    auto endDateTimeJSON{
        codec::iso8601::OptDateTime::encode(recurrence.getEndDateTime())};

    const auto assembleJSON{
        [](std::string_view patternType, const std::string &pattern,
           const std::string &start, const std::string &end) -> nlohmann::json {
            return {{getFieldName(Field::type), patternType},
                    {getFieldName(Field::pattern), pattern},
                    {getFieldName(Field::start), start},
                    {getFieldName(Field::end), end}};
        }};

    switch (recurrence.getPatternType()) {
    case clndr::rec::Recurrence::PatternType::Null:
        return assembleJSON(nullPatternTypeValue,
                            std::string(nullPatternTypeValue),
                            startDateTimeJSON, endDateTimeJSON);

    case clndr::rec::Recurrence::PatternType::Interval:
        return assembleJSON(intervalPatternTypeValue,
                            codec::json::rec::IntervalPattern::encode(
                                recurrence.getIntervalPattern()),
                            startDateTimeJSON, endDateTimeJSON);

    case clndr::rec::Recurrence::PatternType::Weekdays:
        return assembleJSON(weekdayPatternTypeValue,
                            codec::json::rec::WeekdaysPattern::encode(
                                recurrence.getWeekdaysPattern()),
                            startDateTimeJSON, endDateTimeJSON);

    default:
        throw std::runtime_error(
            Error::getMessage(Error::Code::UnsupportedPatternType));
    }
}

[[nodiscard]] auto Recurrence::decode(const nlohmann::json &json)
    -> std::expected<clndr::rec::Recurrence, Error::Code> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::Code::MissingRequiredField);
    }

    auto startDateTimeFromJSON{codec::iso8601::DateTime::decode(
        json[getFieldName(Field::start)].get<std::string>())};
    if (!startDateTimeFromJSON) {
        return std::unexpected(Error::Code::FailedToParseStartDateTime);
    }

    auto endDateTimeFromJSON{codec::iso8601::OptDateTime::decode(
        json[getFieldName(Field::end)].get<std::string>())};
    if (!endDateTimeFromJSON) {
        return std::unexpected(Error::Code::FailedToParseEndDateTime);
    }

    try {
        clndr::rec::Recurrence::Validator::endAfterStart(
            endDateTimeFromJSON.value(), startDateTimeFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(
            Error::Code::ParsedEndDateTimeBeforeStartDateTime);
    }

    auto patternType{json[getFieldName(Field::pattern)].get<std::string>()};

    if (patternType == nullPatternTypeValue) {
        auto pattern{codec::json::rec::NullPattern::decode(
            json[getFieldName(Field::pattern)])};

        if (!pattern) {
            return std::unexpected(Error::Code::FailedToParseNullPattern);
        }

        return clndr::rec::Recurrence::fromValidated(
            pattern.value(), startDateTimeFromJSON.value(),
            endDateTimeFromJSON.value());
    }

    if (patternType == intervalPatternTypeValue) {
        auto pattern{codec::json::rec::IntervalPattern::decode(
            json[getFieldName(Field::pattern)])};

        if (!pattern) {
            return std::unexpected(Error::Code::FailedToParseIntervalPattern);
        }

        return clndr::rec::Recurrence::fromValidated(
            pattern.value(), startDateTimeFromJSON.value(),
            endDateTimeFromJSON.value());
    }

    if (patternType == weekdayPatternTypeValue) {
        auto pattern{codec::json::rec::WeekdaysPattern::decode(
            json[getFieldName(Field::pattern)])};

        if (!pattern) {
            return std::unexpected(Error::Code::FailedToParseWeekdayPattern);
        }

        return clndr::rec::Recurrence::fromValidated(
            pattern.value(), startDateTimeFromJSON.value(),
            endDateTimeFromJSON.value());
    }

    return std::unexpected(Error::Code::UnsupportedPatternType);
}
}; // namespace clndr::codec::json::rec
