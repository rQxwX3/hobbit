#include <recurrence_patterns.hpp>

namespace hbt::mods::util {
IntervalRecurrence::IntervalRecurrence(hbt::mods::Interval interval)
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

[[nodiscard]] auto
IntervalRecurrence::happensOnDate(hbt::mods::DateTime startDate,
                                  hbt::mods::DateTime datetime) const -> bool {
    if (interval_.isZero() && !DateTime::equalDates(startDate, datetime)) {
        return false;
    }

    if (interval_.isLessThanDay()) {
        return true;
    }

    for (auto currDate{startDate}; currDate <= datetime;
         currDate += interval_) {
        if (currDate.getDate() == datetime.getDate()) {
            return true;
        }
    }

    return false;
}

WeekdayRecurrence::WeekdayRecurrence(const hbt::mods::Interval &interval,
                                     hbt::mods::Weekdays weekdays)
    : weekdays_{weekdays} {
    if (!interval.onlyContainsUnit(hbt::mods::Interval::unit_t::WEEK)) {
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
WeekdayRecurrence::getDateOfFirstOccurrence(mods::DateTime startDate) const
    -> hbt::mods::DateTime {
    for (auto days{0}; days != DurationUnits::daysInWeek; ++days) {
        auto date{startDate + Interval::days(days)};

        if (weekdays_.containsWeekday(date.getWeekday())) {
            return date;
        }
    }

    // TODO: assert this function always returns a value (through ctor or
    // otherwise)
}

[[nodiscard]] auto
WeekdayRecurrence::happensOnDate(hbt::mods::DateTime datetime,
                                 hbt::mods::DateTime startDate) const -> bool {
    if (weekdays_.containsWeekday(datetime.getWeekday())) {
        return false;
    }

    auto intervalDurationUnits{interval_.getDurationUnits()};
    auto dateOfFirstOccurrence{getDateOfFirstOccurrence(startDate)};

    return (datetime - dateOfFirstOccurrence)
        .isMultipleOf(intervalDurationUnits);
}
} // namespace hbt::mods::util
