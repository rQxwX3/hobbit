#include <weekdays_pattern.hpp>

namespace hbt::mods::util {
auto WeekdaysRecurrencePattern::validateInterval(const Interval &interval)
    -> Interval {
    if (!interval.onlyContainsUnit(Interval::Unit::WEEK)) {
        throw std::invalid_argument(errorMessage(Error::InvalidInterval));
    }

    return interval;
}

auto WeekdaysRecurrencePattern::validateWeek(const Week &week) -> Week {
    if (week.isEmpty()) {
        throw std::invalid_argument(errorMessage(Error::EmptyWeek));
    }

    return week;
}

WeekdaysRecurrencePattern::WeekdaysRecurrencePattern(DateTime start, Week week,
                                                     Interval interval)
    : firstCalendarWeek_{clndr::Week(
          getFirstOccurrence(start, validateWeek(week)))},
      interval_{validateInterval(interval)}, week_{week} {}

WeekdaysRecurrencePattern::WeekdaysRecurrencePattern(
    clndr::Week firstCalendarWeek, Week week, Interval interval)
    : firstCalendarWeek_{firstCalendarWeek},
      interval_{validateInterval(interval)}, week_{validateWeek(week)} {}

[[nodiscard]] auto WeekdaysRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::getFirstCalendarWeek() const
    -> clndr::Week {
    return firstCalendarWeek_;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::getWeek() const -> Week {
    return week_;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::getFirstOccurrence(DateTime start,
                                                                 Week week)
    -> DateTime {
    assert(!week.isEmpty());

    auto firstOccurrence{start};

    while (!week.containsWeekday(firstOccurrence.getWeekday())) {
        firstOccurrence += Interval::days(1);
    }

    return firstOccurrence;
}

[[nodiscard]] auto WeekdaysRecurrencePattern::happensOnDate(DateTime on) const
    -> bool {
    const auto onWD{on.getWeekday()};

    if (!week_.containsWeekday(onWD)) {
        return false;
    }

    auto firstInstanceOfWeekday(firstCalendarWeek_[onWD]);
    auto daysDiff{DateTime::daysDiff(firstInstanceOfWeekday, on)};

    return daysDiff.isZero() ||
           (on > firstInstanceOfWeekday && daysDiff.isMultipleOf(interval_));
}

[[nodiscard]] auto WeekdaysRecurrencePattern::getOccurrencesOfDate(
    DateTime date, DateTime start) const -> occurrences_t {
    if (happensOnDate(date)) {
        return {DateTime(date)};
    }

    return {};
}

/* --------- JSON --------- */
[[nodiscard]] auto
WeekdaysRecurrencePattern::JSON::containsAllFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(
        JSON::fields,
        [&json](const auto &field) -> bool { return json.contains(field); });
}

[[nodiscard]] auto WeekdaysRecurrencePattern::JSON::encode(
    const WeekdaysRecurrencePattern &pattern) -> nlohmann::json {
    return {
        {JSON::firstCalendarWeekField, pattern.getFirstCalendarWeek().toJSON()},
        {JSON::intervalField, pattern.getInterval().toJSON()},
        {JSON::weekField, pattern.getWeek().toJSON()},
    };
}

[[nodiscard]] auto
WeekdaysRecurrencePattern::JSON::decode(const nlohmann::json &json)
    -> std::expected<WeekdaysRecurrencePattern, Error> {
    if (!containsAllFields(json)) {
        return std::unexpected(JSON::Error::MissingRequiredField);
    }

    auto firstWeekFromJSON =
        clndr::Week::fromJSON(json[firstCalendarWeekField]);
    if (!firstWeekFromJSON) {
        return std::unexpected(JSON::Error::FailedToParseFirstCalendarWeek);
    }

    auto intervalFromJSON = Interval::fromJSON(json[intervalField]);
    if (!intervalFromJSON) {
        return std::unexpected(JSON::Error::FailedToParseInterval);
    }

    auto weekFromJSON = Week::fromJSON(json[weekField]);
    if (!weekFromJSON) {
        return std::unexpected(JSON::Error::FailedToParseWeek);
    }

    try {
        validateInterval(intervalFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(JSON::Error::FailedToValidateInterval);
    }

    try {
        validateWeek(weekFromJSON.value());
    } catch (std::invalid_argument) {
        return std::unexpected(JSON::Error::FailedToValidateWeek);
    }

    return WeekdaysRecurrencePattern(firstWeekFromJSON.value(),
                                     weekFromJSON.value(),
                                     intervalFromJSON.value());
}
} // namespace hbt::mods::util
