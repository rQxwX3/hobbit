#include <duration_units.hpp>

namespace hbt::mods::util {
DurationUnits::DurationUnits() : units_{array_t{}} {}

DurationUnits::DurationUnits(array_t units) : units_{units} {}

auto DurationUnits::addYears(value_t value) -> void {
    units_[unit_t::YEAR] += value;
}

auto DurationUnits::addMonths(value_t value) -> void {
    this->addYears(value / DurationUnits::monthsInYear);

    units_[unit_t::MONTH] += value % DurationUnits::monthsInYear;
}

auto DurationUnits::addWeeks(value_t value) -> void {
    this->addMonths(value / DurationUnits::weeksInMonth);

    units_[unit_t::WEEK] += value % DurationUnits::weeksInMonth;
}

auto DurationUnits::addDays(value_t value) -> void {
    this->addWeeks(value / DurationUnits::daysInWeek);

    units_[unit_t::DAY] += value % DurationUnits::daysInWeek;
}

auto DurationUnits::addHours(value_t value) -> void {
    this->addDays(value / DurationUnits::hoursInDay);

    units_[unit_t::HOUR] += value % DurationUnits::hoursInDay;
}

auto DurationUnits::addMinutes(value_t value) -> void {
    this->addHours(value / DurationUnits::minutesInHour);

    units_[unit_t::MINUTE] += value % DurationUnits::minutesInHour;
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

[[nodiscard]] auto DurationUnits::operator+(const DurationUnits &other)
    -> DurationUnits {
    auto result{DurationUnits{*this}};

    result.addYears(other.getYears());
    result.addMonths(other.getMonths());
    result.addWeeks(other.getWeeks());
    result.addDays(other.getDays());
    result.addHours(other.getHours());
    result.addMinutes(other.getMinutes());

    return result;
}
} // namespace hbt::mods::util
