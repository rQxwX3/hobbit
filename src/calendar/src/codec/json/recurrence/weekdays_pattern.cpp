#include <codec/json/datetime/interval.hpp>
#include <codec/json/datetime/week.hpp>
#include <codec/json/recurrence/selected_weekdays.hpp>
#include <codec/json/recurrence/weekdays_pattern.hpp>

namespace clndr::codec::json::rec {
[[nodiscard]] auto
WeekdaysPattern::encode(const clndr::rec::WeekdaysPattern &pattern)
    -> nlohmann::json {
    return {
        {getFieldName(Field::firstWeek),
         codec::json::dt::Week::encode(pattern.getFirstWeek())},
        {getFieldName(Field::interval),
         codec::json::dt::Interval::encode(pattern.getInterval())},
        {getFieldName(Field::selectedWeekdays),
         codec::json::rec::SelectedWeekdays::encode(
             pattern.getSelectedWeekdays())},
    };
}

[[nodiscard]] auto WeekdaysPattern::decode(const nlohmann::json &json)
    -> std::expected<clndr::rec::WeekdaysPattern, Error::Code> {
    if (!containsAllFields(json)) {
        return std::unexpected(Error::Code::MissingRequiredField);
    }

    auto firstWeekFromJSON =
        codec::json::dt::Week::decode(json[getFieldName(Field::firstWeek)]);
    if (!firstWeekFromJSON) {
        return std::unexpected(Error::Code::FailedToParseFirstWeek);
    }

    auto intervalFromJSON =
        codec::json::dt::Interval::decode(json[getFieldName(Field::interval)]);
    if (!intervalFromJSON) {
        return std::unexpected(Error::Code::FailedToParseInterval);
    }

    auto selectedWDsFromJSON = codec::json::rec::SelectedWeekdays::decode(
        json[getFieldName(Field::selectedWeekdays)]);
    if (!selectedWDsFromJSON) {
        return std::unexpected(Error::Code::FailedToParseWeek);
    }

    try {
        clndr::rec::WeekdaysPattern::validateInterval(intervalFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::Code::FailedToValidateInterval);
    }

    try {
        clndr::rec::WeekdaysPattern::validateSelectedWeekdays(
            selectedWDsFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(Error::Code::FailedToValidateWeek);
    }

    return clndr::rec::WeekdaysPattern(firstWeekFromJSON.value(),
                                       selectedWDsFromJSON.value(),
                                       intervalFromJSON.value());
}
} // namespace clndr::codec::json::rec
