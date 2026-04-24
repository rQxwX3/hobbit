#include <recurrence_patterns.hpp>

namespace hbt::mods::util {
IntervalRecurrencePattern::IntervalRecurrencePattern(const Interval &interval)
    : interval_{interval} {}

[[nodiscard]] auto IntervalRecurrencePattern::toJSON() const -> nlohmann::json {
    return interval_.toJSON();
};

[[nodiscard]] auto
IntervalRecurrencePattern::fromJSON(const nlohmann::json &json)
    -> std::expected<IntervalRecurrencePattern, Error> {
    auto intervalFromJSON{Interval::fromJSON(json)};
    if (!intervalFromJSON) {
        return std::unexpected(Error::JSONFailedToParseInterval);
    }

    return IntervalRecurrencePattern(intervalFromJSON.value());
}

[[nodiscard]] auto IntervalRecurrencePattern::getInterval() const -> Interval {
    return interval_;
}

[[nodiscard]] auto
IntervalRecurrencePattern::happensOnDate(mods::DateTime start,
                                         mods::Date date) const -> bool {
    if (interval_.isZero()) {
        return start.getDate() == date;
    }

    if (interval_.getDuration() < Duration::days(1)) {
        return true;
    }

    // TODO: for day-based intervals use math instead of loop

    for (auto dt{start}; dt.getDate() <= date; dt += interval_) {
        if (dt.getDate() == date) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto
IntervalRecurrencePattern::getFirstOccurrencesOnDate(mods::DateTime start,
                                                     mods::Date date) const
    -> std::optional<occurrence_t> {
    for (auto dt{start}; dt.getDate() <= date; dt += interval_) {
        if (dt.getDate() == date) {
            return dt;
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto IntervalRecurrencePattern::getOccurrencesOnDate(
    mods::DateTime start, mods::Date date) const -> occurrences_t {
    auto result{occurrences_t{}};

    auto firstOccurrence{getFirstOccurrencesOnDate(start, date)};
    if (!firstOccurrence.has_value()) {
        return result;
    }

    auto endDate{date + mods::Duration::days(1)};

    for (auto dt{firstOccurrence}; dt->getDate() != endDate; *dt += interval_) {
        result.push_back(*dt);
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
WeekdayRecurrencePattern::getDateOfFirstOccurrence(mods::DateTime start) const
    -> occurrence_t {
    for (auto days{0}; days != Duration::daysInWeek; ++days) {
        auto date{start + Duration::days(days)};

        if (weekdays_.containsWeekday(date.getDate().getWeekday())) {
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

    auto intervalDuration{interval_.getDuration()};
    auto firstOccurrenceDate{getDateOfFirstOccurrence(start)};

    return Date::getDiff(date, firstOccurrenceDate.getDate())
        .isMultipleOf(intervalDuration);
}

[[nodiscard]] auto WeekdayRecurrencePattern::getOccurrencesOnDate(
    mods::DateTime start, mods::Date date) const -> occurrences_t {
    if (happensOnDate(start, date)) {
        return {mods::DateTime(date)};
    }

    return {};
}

[[nodiscard]] auto WeekdayRecurrencePattern::toJSON() const -> nlohmann::json {
    return {{jsonIntervalField, interval_.toJSON()},
            {jsonWeekdaysField, weekdays_.toJSON()}};
}

[[nodiscard]] auto
WeekdayRecurrencePattern::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto
WeekdayRecurrencePattern::fromJSON(const nlohmann::json &json)
    -> std::expected<WeekdayRecurrencePattern, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto intervalFromJSON{Interval::fromJSON(json[jsonIntervalField])};
    if (!intervalFromJSON) {
        return std::unexpected(Error::JSONFailedToParseInterval);
    }

    auto weekdaysFromJSON{Weekdays::fromJSON(json[jsonWeekdaysField])};
    if (!weekdaysFromJSON) {
        return std::unexpected(Error::JSONFailedToParseWeekdays);
    }

    return WeekdayRecurrencePattern{intervalFromJSON.value(),
                                    weekdaysFromJSON.value()};
}
} // namespace hbt::mods::util
