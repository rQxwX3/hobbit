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

WeekdaysRecurrencePattern::WeekdaysRecurrencePattern(const Interval &interval,
                                                     Weekdays weekdays)
    : weekdays_{validateWeekdays(weekdays)},
      interval_{validateInterval(interval)} {}

[[nodiscard]] auto WeekdaysRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::getWeekdays() const -> Weekdays {
    return weekdays_;
}

[[nodiscard]] auto
WeekdaysRecurrencePattern::getFirstOccurrence(DateTime start) const
    -> occurrence_t {
    for (auto days{0}; days != Interval::daysInWeek; ++days) {
        auto datetime{start + Interval::days(days)};

        if (weekdays_.containsWeekday(datetime.getWeekday())) {
            return datetime;
        }
    }

    throw std::runtime_error(errorMessage(Error::EmptyWeekdays));
}

[[nodiscard]] auto WeekdaysRecurrencePattern::happensOnDate(DateTime start,
                                                            DateTime on) const
    -> bool {
    if (!weekdays_.containsWeekday(on.getWeekday())) {
        return false;
    }

    auto firstTimeStampDateTime{getFirstOccurrence(start)};

    return DateTime::daysDiff(on, firstTimeStampDateTime)
        .isMultipleOf(interval_);
}

[[nodiscard]] auto WeekdaysRecurrencePattern::getOccurrencesOfDate(
    DateTime start, DateTime on) const -> occurrences_t {
    if (happensOnDate(start, on)) {
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

    auto intervalFromISO8601{Interval::fromISO8601String(
        json[jsonIntervalField].get<std::string>())};
    if (!intervalFromISO8601) {
        return std::unexpected(Error::JSONFailedToParseInterval);
    }

    auto weekdaysFromJSON{Weekdays::fromJSON(json[jsonWeekdaysField])};
    if (!weekdaysFromJSON) {
        return std::unexpected(Error::JSONFailedToParseWeekdays);
    }

    return WeekdaysRecurrencePattern{intervalFromISO8601.value(),
                                     weekdaysFromJSON.value()};
}
} // namespace hbt::mods::util
