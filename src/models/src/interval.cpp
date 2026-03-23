#include <interval.hpp>

#include <string>

namespace hbt::mods {
Interval::Interval(MonthHandling monthHandling)
    : durationUnits_{hbt::mods::DurationUnits{}},
      monthHandling_{monthHandling} {}

Interval::Interval(hbt::mods::DurationUnits durationUnits,
                   MonthHandling monthHandling)
    : durationUnits_{durationUnits}, monthHandling_{monthHandling} {}

[[nodiscard]] auto Interval::years(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addYears(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::months(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addMonths(value);

    return Interval{durationUnits, MonthHandling::CUT_OFF};
}

[[nodiscard]] auto Interval::weeks(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addWeeks(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::days(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addDays(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::hours(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addHours(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::minutes(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addMinutes(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::getYears() const -> value_t {
    return durationUnits_.getYears();
}

[[nodiscard]] auto Interval::getMonths() const -> value_t {
    return durationUnits_.getMonths();
}

[[nodiscard]] auto Interval::getWeeks() const -> value_t {
    return durationUnits_.getWeeks();
}

[[nodiscard]] auto Interval::getDays() const -> value_t {
    return durationUnits_.getDays();
}

[[nodiscard]] auto Interval::getHours() const -> value_t {
    return durationUnits_.getHours();
}

[[nodiscard]] auto Interval::getMinutes() const -> value_t {
    return durationUnits_.getMinutes();
}

[[nodiscard]] auto Interval::getMonthHandling() const -> MonthHandling {
    return monthHandling_;
}

auto Interval::setMonthHandling(MonthHandling monthHandling) -> void {
    monthHandling_ = monthHandling;
}

[[nodiscard]] auto Interval::operator+(const Interval &other) const
    -> Interval {
    return Interval{this->durationUnits_ + other.durationUnits_};
}

[[nodiscard]] auto Interval::isZero() const -> bool {
    return durationUnits_.isZero();
}
[[nodiscard]] auto Interval::toJSON() const -> nlohmann::json {
    return {
        {"duration_units", durationUnits_.toISO8601String()},
        {"month_handling", std::to_string(static_cast<int>(monthHandling_))}};
}

[[nodiscard]] auto Interval::fromJSON(const nlohmann::json &json)
    -> std::optional<Interval> {
    if (!json.contains("duration_units") || !json.contains("month_handling")) {
        return std::nullopt;
    }

    auto durationUnitsFromISO8601String{
        hbt::mods::DurationUnits::fromISO8601String(
            json["duration_units"].get<std::string>())};

    if (!durationUnitsFromISO8601String.has_value()) {
        return std::nullopt;
    }

    auto monthHandling{static_cast<Interval::MonthHandling>(
        std::stoi(json["month_handling"].get<std::string>()))};

    return Interval{durationUnitsFromISO8601String.value(), monthHandling};
}
} // namespace hbt::mods
