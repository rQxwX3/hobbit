#include <interval.hpp>

namespace hbt::mods {
Interval::Interval() = default;

Interval::Interval(hbt::mods::util::DurationUnits durationUnits)
    : durationUnits_{durationUnits} {}

[[nodiscard]] auto Interval::years(value_t value) -> Interval {
    auto durationUnits{hbt::mods::util::DurationUnits{}};

    durationUnits.addYears(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::months(value_t value) -> Interval {
    auto durationUnits{hbt::mods::util::DurationUnits{}};

    durationUnits.addMonths(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::weeks(value_t value) -> Interval {
    auto durationUnits{hbt::mods::util::DurationUnits{}};

    durationUnits.addWeeks(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::days(value_t value) -> Interval {
    auto durationUnits{hbt::mods::util::DurationUnits{}};

    durationUnits.addDays(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::hours(value_t value) -> Interval {
    auto durationUnits{hbt::mods::util::DurationUnits{}};

    durationUnits.addHours(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::minutes(value_t value) -> Interval {
    auto durationUnits{hbt::mods::util::DurationUnits{}};

    durationUnits.addMinutes(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::operator+(const Interval &other) const
    -> Interval {
    return Interval{this->durationUnits_ + other.durationUnits_};
}

[[nodiscard]] auto Interval::isZero() const -> bool {
    return durationUnits_.isZero();
}

[[nodiscard]] auto Interval::toISO8601String() const -> std::string {
    return durationUnits_.toISO8601String();
}

[[nodiscard]] auto Interval::fromISO8601String(const std::string &string)
    -> std::optional<Interval> {
    auto durationUnitsFromISO8601String{
        hbt::mods::util::DurationUnits::fromISO8601String(string)};

    if (durationUnitsFromISO8601String.has_value()) {
        return Interval{durationUnitsFromISO8601String.value()};
    }

    return std::nullopt;
}
} // namespace hbt::mods
