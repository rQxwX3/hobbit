#include <recurrence_patterns.hpp>

namespace hbt::mods::util {
IntervalRecurrence::IntervalRecurrence(const hbt::mods::Interval &interval)
    : interval_{interval} {}

[[nodiscard]] auto IntervalRecurrence::isValidJSON(const nlohmann::json &json)
    -> bool {
    return json.contains("type") && json["type"] != typeJSON &&
           json.contains("interval");
}

[[nodiscard]] auto IntervalRecurrence::toJSON() const -> nlohmann::json {
    return {{"type", typeJSON}, {"interval", interval_.toJSON()}};
};

[[nodiscard]] auto IntervalRecurrence::fromJSON(const nlohmann::json &json)
    -> std::optional<IntervalRecurrence> {
    if (!isValidJSON(json)) {
        return std::nullopt;
    }

    auto intervalFromJSON{hbt::mods::Interval::fromJSON(json["interval"])};
    if (!intervalFromJSON.has_value()) {
        return std::nullopt;
    }

    return IntervalRecurrence(intervalFromJSON.value());
}

[[nodiscard]] auto IntervalRecurrence::getInterval() const
    -> hbt::mods::Interval {
    return interval_;
}

[[nodiscard]] auto IntervalRecurrence::happensOnDate(mods::DateTime start,
                                                     mods::Date date) const
    -> bool {
    if (interval_.isZero() && start.getDate() != date) {
        return false;
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
IntervalRecurrence::getFirstOccurrencesOnDate(mods::DateTime start,
                                              mods::Date date) const
    -> std::optional<occurrence_t> {
    for (auto dt{start}; dt.getDate() <= date; dt += interval_) {
        if (dt.getDate() == date) {
            return dt;
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto IntervalRecurrence::getOccurrencesOnDate(
    mods::DateTime start, mods::Date date) const -> occurrences_t {
    auto result{occurrences_t{}};

    auto firstOccurrence{getFirstOccurrencesOnDate(start, date)};
    if (!firstOccurrence.has_value()) {
        return result;
    }

    auto nextDay{date + mods::Duration::days(1)};

    for (auto dt{firstOccurrence}; dt->getDate() != nextDay; *dt += interval_) {
        result.push_back(*dt);
    }

    return result;
}

WeekdayRecurrence::WeekdayRecurrence(const mods::Interval &interval,
                                     mods::Weekdays weekdays)
    : weekdays_{weekdays} {
    if (!interval.onlyContainsUnit(mods::Interval::unit_t::WEEK)) {
        throw std::invalid_argument(
            "Only weekly intervals are accepted by weekday recurrence");
    }

    interval_ = interval;
}

[[nodiscard]] auto WeekdayRecurrence::isValidJSON(const nlohmann::json &json)
    -> bool {
    return json.contains("type") && json["type"] == typeJSON &&
           json.contains("interval") && json.contains("weekdays");
}

[[nodiscard]] auto WeekdayRecurrence::toJSON() const -> nlohmann::json {
    return {{"type", typeJSON},
            {"interval", interval_.toJSON()},
            {"weekdays", weekdays_.toJSON()}};
}

[[nodiscard]] auto WeekdayRecurrence::fromJSON(const nlohmann::json &json)
    -> std::optional<WeekdayRecurrence> {
    if (!isValidJSON(json)) {
        return std::nullopt;
    }

    auto intervalFromJSON{Interval::fromJSON(json["interval"])};
    if (!intervalFromJSON.has_value()) {
        return std::nullopt;
    }

    auto weekdaysFromJSON{Weekdays::fromJSON(json["weekdays"])};
    if (!weekdaysFromJSON.has_value()) {
        return std::nullopt;
    }

    return WeekdayRecurrence{intervalFromJSON.value(),
                             weekdaysFromJSON.value()};
}

[[nodiscard]] auto WeekdayRecurrence::getInterval() const
    -> hbt::mods::Interval {
    return interval_;
}

[[nodiscard]] auto WeekdayRecurrence::getWeekdays() const
    -> hbt::mods::Weekdays {
    return weekdays_;
}

[[nodiscard]] auto
WeekdayRecurrence::getDateOfFirstOccurrence(mods::DateTime start) const
    -> occurrence_t {
    for (auto days{0}; days != Duration::daysInWeek; ++days) {
        auto date{start + Duration::days(days)};

        if (weekdays_.containsWeekday(date.getDate().getWeekday())) {
            return date;
        }
    }

    // TODO: assert this function always returns a value (through ctor or
    // otherwise)
}

[[nodiscard]] auto WeekdayRecurrence::happensOnDate(hbt::mods::DateTime start,
                                                    hbt::mods::Date date) const
    -> bool {
    if (weekdays_.containsWeekday(date.getWeekday())) {
        return false;
    }

    auto intervalDuration{interval_.getDuration()};
    auto dateOfFirstOccurrence{getDateOfFirstOccurrence(start)};

    return Date::getDiff(date, dateOfFirstOccurrence.getDate())
        .isMultipleOf(intervalDuration);
}

[[nodiscard]] auto WeekdayRecurrence::getOccurrencesOnDate(
    mods::DateTime start, mods::Date date) const -> occurrences_t {
    if (happensOnDate(start, date)) {
        return {mods::DateTime(date)};
    }

    return {};
}
} // namespace hbt::mods::util
