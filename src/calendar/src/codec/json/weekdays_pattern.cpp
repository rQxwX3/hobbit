#include <codec/json/interval.hpp>
#include <codec/json/week.hpp>
#include <codec/json/weekdays_pattern.hpp>

namespace clndr::codec::json {
[[nodiscard]] auto WeekdaysPattern::encode(const rec::WeekdaysPattern &pattern)
    -> nlohmann::json {
    return {
        {getFieldName(Field::firstWeek),
         codec::json::Week::encode(pattern.getFirstWeek())},
        {getFieldName(Field::interval),
         codec::json::Interval::encode(pattern.getInterval())},
        {getFieldName(Field::selectedWeekdays),
         pattern.getSelectedWeekdays().toJSON()},
    };
}

[[nodiscard]] auto WeekdaysPattern::decode(const nlohmann::json &json)
    -> std::expected<rec::WeekdaysPattern, Error::Code> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::Code::MissingRequiredField);
    }

    auto firstWeekFromJSON =
        codec::json::Week::decode(json[getFieldName(Field::firstWeek)]);
    if (!firstWeekFromJSON) {
        return std::unexpected(Error::Code::FailedToParseFirstWeek);
    }

    auto intervalFromJSON =
        codec::json::Interval::decode(json[getFieldName(Field::interval)]);
    if (!intervalFromJSON) {
        return std::unexpected(Error::Code::FailedToParseInterval);
    }

    auto selectedWDsFromJSON = rec::SelectedWeekdays::fromJSON(
        json[getFieldName(Field::selectedWeekdays)]);
    if (!selectedWDsFromJSON) {
        return std::unexpected(Error::Code::FailedToParseWeek);
    }

    try {
        rec::WeekdaysPattern::validateInterval(intervalFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::Code::FailedToValidateInterval);
    }

    try {
        rec::WeekdaysPattern::validateSelectedWeekdays(
            selectedWDsFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::Code::FailedToValidateWeek);
    }

    return rec::WeekdaysPattern(firstWeekFromJSON.value(),
                                selectedWDsFromJSON.value(),
                                intervalFromJSON.value());
}
} // namespace clndr::codec::json
