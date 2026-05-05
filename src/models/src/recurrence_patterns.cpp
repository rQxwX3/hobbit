#include <recurrence_patterns.hpp>

namespace hbt::mods::util {
auto IntervalRecurrencePattern::validateInterval(const Interval &interval)
    -> Interval {
    if (interval.isZero()) {
        throw std::invalid_argument(errorMessage(Error::InvalidInterval));
    }

    return interval;
}

IntervalRecurrencePattern::IntervalRecurrencePattern(const Interval &interval)
    : interval_{validateInterval(interval)} {}

[[nodiscard]] auto IntervalRecurrencePattern::toJSON() const -> nlohmann::json {
    return interval_.toISO8601String();
};

[[nodiscard]] auto
IntervalRecurrencePattern::fromJSON(const nlohmann::json &json)
    -> std::expected<IntervalRecurrencePattern, Error> {
    auto intervalFromISO8601{
        Interval::fromISO8601String(json.get<std::string>())};
    if (!intervalFromISO8601) {
        return std::unexpected(Error::JSONFailedToParseInterval);
    }

    return IntervalRecurrencePattern(intervalFromISO8601.value());
}

[[nodiscard]] auto IntervalRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto
IntervalRecurrencePattern::happensOnDate(mods::DateTime start,
                                         mods::Date date) const -> bool {
    if (interval_.isZero()) {
        return start.getDaysSinceEpoch() == date;
    }

    if (interval_ < Interval::days(1)) {
        return true;
    }

    // TODO: for day-based intervals use math instead of a loop

    for (auto dt{start}; dt.getDaysSinceEpoch() <= date; dt += interval_) {
        if (dt.getDaysSinceEpoch() == date) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto IntervalRecurrencePattern::getFirstTimeStampOnDate(
    mods::DateTime start, mods::Date date) const -> std::optional<timestamp_t> {
    for (auto dt{start}; dt.getDaysSinceEpoch() <= date; dt += interval_) {
        if (dt.getDaysSinceEpoch() == date) {
            return dt;
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto IntervalRecurrencePattern::getTimeStampsOnDate(
    mods::DateTime start, mods::Date date) const -> timestamps_t {
    auto result{timestamps_t{}};

    auto firstTS{getFirstTimeStampOnDate(start, date)};
    if (!firstTS.has_value()) {
        return result;
    }

    auto endDate{date + mods::Interval::days(1)};

    for (auto ts{firstTS}; ts->getDays() != endDate; *ts += interval_) {
        result.push_back(*ts);
    }

    return result;
}

auto WeekdayRecurrencePattern::validateInterval(const Interval &interval)
    -> Interval {
    if (!interval.onlyContainsUnit(mods::Interval::unit_t::WEEK)) {
        throw std::invalid_argument(errorMessage(Error::InvalidInterval));
    }

    return interval;
}

auto WeekdayRecurrencePattern::validateWeekdays(Weekdays weekdays) -> Weekdays {
    if (weekdays.getDays().none()) {
        throw std::invalid_argument(errorMessage(Error::InvalidWeekdays));
    }

    return weekdays;
}

WeekdayRecurrencePattern::WeekdayRecurrencePattern(
    const mods::Interval &interval, mods::Weekdays weekdays)
    : weekdays_{validateWeekdays(weekdays)},
      interval_{validateInterval(interval)} {}

[[nodiscard]] auto WeekdayRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto WeekdayRecurrencePattern::getWeekdays() const -> Weekdays {
    return weekdays_;
}

[[nodiscard]] auto
WeekdayRecurrencePattern::getDateOfFirstTimeStamp(mods::DateTime start) const
    -> timestamp_t {
    for (auto days{0}; days != Interval::daysInWeek; ++days) {
        auto date{start + Interval::days(days)};

        if (weekdays_.containsWeekday(date.getDaysSinceEpoch().getWeekday())) {
            return date;
        }
    }

    throw std::runtime_error(errorMessage(Error::EmptyWeekdays));
}

[[nodiscard]] auto WeekdayRecurrencePattern::happensOnDate(DateTime start,
                                                           Date date) const
    -> bool {
    if (!weekdays_.containsWeekday(date.getWeekday())) {
        return false;
    }

    auto firstTimeStampDate{getDateOfFirstTimeStamp(start)};

    return Date::daysBetween(date, firstTimeStampDate.getDays())
        .isMultipleOf(interval_);
}

[[nodiscard]] auto WeekdayRecurrencePattern::getTimeStampsOnDate(
    mods::DateTime start, mods::Date date) const -> timestamps_t {
    if (happensOnDate(start, date)) {
        return {mods::DateTime(date)};
    }

    return {};
}

[[nodiscard]] auto WeekdayRecurrencePattern::toJSON() const -> nlohmann::json {
    return {{jsonIntervalField, interval_.toISO8601String()},
            {jsonWeekdaysField, weekdays_.toJSON()}};
}

[[nodiscard]] auto
WeekdayRecurrencePattern::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto
WeekdayRecurrencePattern::fromJSON(const nlohmann::json &json)
    -> std::expected<WeekdayRecurrencePattern, Error> {
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

    return WeekdayRecurrencePattern{intervalFromISO8601.value(),
                                    weekdaysFromJSON.value()};
}
} // namespace hbt::mods::util
