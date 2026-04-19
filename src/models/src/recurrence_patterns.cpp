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
} // namespace hbt::mods::util
