#include <codec/iso8601/datetime.hpp>
#include <codec/iso8601/opt_datetime.hpp>
#include <codec/json/recurrence.hpp>
#include <error.hpp>
#include <json.hpp>
#include <recurrence/recurrence.hpp>

namespace clndr::codec::json {
[[nodiscard]] auto Recurrence::encode(const rec::Recurrence &recurrence)
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
    case rec::Recurrence::PatternType::Null:
        return assembleJSON(nullPatternTypeValue,
                            std::string(nullPatternTypeValue),
                            startDateTimeJSON, endDateTimeJSON);

    case rec::Recurrence::PatternType::Interval:
        return assembleJSON(
            intervalPatternTypeValue,
            rec::IntervalPattern::JSON::encode(recurrence.getIntervalPattern()),
            startDateTimeJSON, endDateTimeJSON);

    case rec::Recurrence::PatternType::Weekdays:
        return assembleJSON(
            weekdayPatternTypeValue,
            rec::WeekdaysPattern::JSON::encode(recurrence.getWeekdaysPattern()),
            startDateTimeJSON, endDateTimeJSON);

    default:
        throw std::runtime_error(
            Error::getMessage(Error::Code::UnsupportedPatternType));
    }
}

[[nodiscard]] auto Recurrence::decode(const nlohmann::json &json)
    -> std::expected<rec::Recurrence, Error::Code> {
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
        rec::Recurrence::Validator::endAfterStart(
            endDateTimeFromJSON.value(), startDateTimeFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(
            Error::Code::ParsedEndDateTimeBeforeStartDateTime);
    }

    auto patternType{json[getFieldName(Field::pattern)].get<std::string>()};

    if (patternType == nullPatternTypeValue) {
        auto pattern{
            rec::NullPattern::JSON::decode(json[getFieldName(Field::pattern)])};

        if (!pattern) {
            return std::unexpected(Error::Code::FailedToParseNullPattern);
        }

        return rec::Recurrence::fromValidated(pattern.value(),
                                              startDateTimeFromJSON.value(),
                                              endDateTimeFromJSON.value());
    }

    if (patternType == intervalPatternTypeValue) {
        auto pattern{rec::IntervalPattern::JSON::decode(
            json[getFieldName(Field::pattern)])};

        if (!pattern) {
            return std::unexpected(Error::Code::FailedToParseIntervalPattern);
        }

        return rec::Recurrence::fromValidated(pattern.value(),
                                              startDateTimeFromJSON.value(),
                                              endDateTimeFromJSON.value());
    }

    if (patternType == weekdayPatternTypeValue) {
        auto pattern{rec::WeekdaysPattern::JSON::decode(
            json[getFieldName(Field::pattern)])};

        if (!pattern) {
            return std::unexpected(Error::Code::FailedToParseWeekdayPattern);
        }

        return rec::Recurrence::fromValidated(pattern.value(),
                                              startDateTimeFromJSON.value(),
                                              endDateTimeFromJSON.value());
    }

    return std::unexpected(Error::Code::UnsupportedPatternType);
}
}; // namespace clndr::codec::json
