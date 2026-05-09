#include <weekdays_pattern.hpp>

namespace hbt::mods::util {
auto WeekdaysRecurrencePattern::validateInterval(const Interval &interval)
    -> Interval {
    if (!interval.onlyContainsUnit(Interval::Unit::WEEK)) {
        throw std::invalid_argument(errorMessage(Error::InvalidInterval));
    }

    return interval;
}

auto WeekdaysRecurrencePattern::validateWeekdays(Weekdays weekdays)
    -> Weekdays {
    if (weekdays.getDays().none()) {
        throw std::invalid_argument(errorMessage(Error::InvalidWeekdays));
    }

    return weekdays;
}

WeekdaysRecurrencePattern::WeekdaysRecurrencePattern(DateTime start,
                                                     Interval interval,
                                                     Weekdays weekdays)
    : start_{start}, weekdays_{validateWeekdays(weekdays)},
      interval_{validateInterval(interval)} {}

[[nodiscard]] auto WeekdaysRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::getWeekdays() const -> Weekdays {
    return weekdays_;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::happensOnDate(DateTime on) const
    -> bool {
    if (!weekdays_.containsWeekday(on.getWeekday())) {
        return false;
    }

    return DateTime::daysDiff(start_, on).isMultipleOf(interval_);
}

[[nodiscard]] auto
WeekdaysRecurrencePattern::getOccurrencesOfDate(DateTime on) const
    -> occurrences_t {
    if (happensOnDate(on)) {
        return {DateTime(on)};
    }

    return {};
}

[[nodiscard]] auto WeekdaysRecurrencePattern::toJSON() const -> nlohmann::json {
    return {{jsonIntervalField, interval_.toISO8601String()},
            {jsonWeekdaysField, weekdays_.toJSON()}};
}



[[nodiscard]] auto
WeekdaysRecurrencePattern::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto
WeekdaysRecurrencePattern::fromJSON(const nlohmann::json &json)
    -> std::expected<WeekdaysRecurrencePattern, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto startFromISO8601{
        DateTime::fromISO8601String(json[jsonStartField].get<std::string>())};
    if (!startFromISO8601) {
        return std::unexpected(Error::JSONFailedToParseStart);
    }

    auto intervalFromJSON{Interval::fromJSON(json[jsonIntervalField])};
    if (!intervalFromJSON) {
        return std::unexpected(Error::JSONFailedToParseInterval);
    }

    auto weekdaysFromJSON{Weekdays::fromJSON(json[jsonWeekdaysField])};
    if (!weekdaysFromJSON) {
        return std::unexpected(Error::JSONFailedToParseWeekdays);
    }

    return WeekdaysRecurrencePattern{startFromISO8601.value(),
                                     intervalFromJSON.value(),
                                     weekdaysFromJSON.value()};
}
} // namespace hbt::mods::util
