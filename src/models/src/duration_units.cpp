#include <duration_units.hpp>

namespace hbt::mods::util {
DurationUnits::DurationUnits() : units_{array_t{}} {}

DurationUnits::DurationUnits(array_t units) : units_{units} {}

auto DurationUnits::addYears(value_t value) -> void {
    units_[unit_t::YEAR] += value;
}

auto DurationUnits::addMonths(value_t value) -> void {
    units_[unit_t::MONTH] += value;
}

auto DurationUnits::addWeeks(value_t value) -> void {
    units_[unit_t::WEEK] += value;
}

auto DurationUnits::addDays(value_t value) -> void {
    units_[unit_t::DAY] += value;
}

auto DurationUnits::addHours(value_t value) -> void {
    units_[unit_t::HOUR] += value;
}

auto DurationUnits::addMinutes(value_t value) -> void {
    units_[unit_t::MINUTE] += value;
}

[[nodiscard]] auto DurationUnits::getYears() const -> value_t {
    return units_[unit_t::YEAR];
}

[[nodiscard]] auto DurationUnits::getMonths() const -> value_t {
    return units_[unit_t::MONTH];
}

[[nodiscard]] auto DurationUnits::getWeeks() const -> value_t {
    return units_[unit_t::WEEK];
}

[[nodiscard]] auto DurationUnits::getDays() const -> value_t {
    return units_[unit_t::DAY];
}

[[nodiscard]] auto DurationUnits::getHours() const -> value_t {
    return units_[unit_t::HOUR];
}

[[nodiscard]] auto DurationUnits::getMinutes() const -> value_t {
    return units_[unit_t::MINUTE];
}
} // namespace hbt::mods::util
