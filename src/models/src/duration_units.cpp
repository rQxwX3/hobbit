#include <datetime.hpp>
#include <duration_units.hpp>
#include <duration_units_parser.hpp>

#include <algorithm>
#include <string>

namespace hbt::mods {
DurationUnits::DurationUnits() : units_{array_t{}} {}

DurationUnits::DurationUnits(array_t unitsArray) : units_{unitsArray} {}

DurationUnits::DurationUnits(const Units &unitsStruct)
    : units_{unitsStruct.toArray()} {};

auto DurationUnits::convertUnitsUpwards() -> DurationUnits {
    auto minutesToHours{[this]() -> void {
        units_[unit_t::HOUR] += units_[unit_t::MINUTE] / minutesInHour;
        units_[unit_t::MINUTE] %= minutesInHour;
    }};

    auto hoursToDays{[this]() -> void {
        units_[unit_t::DAY] += units_[unit_t::HOUR] / hoursInDay;
        units_[unit_t::HOUR] %= hoursInDay;
    }};

    auto daysToWeeks{[this]() -> void {
        units_[unit_t::WEEK] += units_[unit_t::DAY] / daysInWeek;
        units_[unit_t::DAY] %= daysInWeek;
    }};

    auto monthsToYears{[this]() -> void {
        units_[unit_t::YEAR] += units_[unit_t::MONTH] / monthsInYear;
        units_[unit_t::MONTH] %= monthsInYear;
    }};

    minutesToHours();
    hoursToDays();
    daysToWeeks();
    monthsToYears();

    return *this;
}

[[nodiscard]] auto DurationUnits::fromUnit(unit_t unit, value_t value)
    -> DurationUnits {
    auto array{array_t{}};
    array[unit] = value;

    return DurationUnits{array}.convertUnitsUpwards();
}

[[nodiscard]] auto DurationUnits::getMaxNonZeroUnit() const
    -> std::optional<unit_t> {
    for (auto unit{static_cast<size_t>(unit_t::YEAR)}; unit != unit_t::COUNT_;
         ++unit) {
        if (units_[unit] != 0) {
            return static_cast<unit_t>(unit);
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto DurationUnits::getNonZeroUnitValuePairs() const
    -> std::vector<unitValuePair_t> {
    std::vector<unitValuePair_t> result;
    result.reserve(unit_t::COUNT_);

    for (auto unit{static_cast<size_t>(unit_t::YEAR)}; unit != unit_t::COUNT_;
         ++unit) {
        if (auto value{units_[unit]}; value) {
            result.emplace_back(static_cast<unit_t>(unit), value);
        }
    }

    return result;
}

auto DurationUnits::addUnit(unit_t unit, value_t value) -> void {
    units_[unit] += value;
}

[[nodiscard]] auto DurationUnits::getUnitValue(unit_t unit) const -> value_t {
    return units_[unit];
}

[[nodiscard]] auto DurationUnits::isLessThanDay() const -> bool {
    if (auto maxUnit{getMaxNonZeroUnit()};
        maxUnit.has_value() && maxUnit > unit_t::HOUR) {
        return false;
    }

    auto time{(getUnitValue(unit_t::HOUR) * DateTime::timeInHour) +
              (getUnitValue(unit_t::MINUTE) * DateTime::timeInMinute)};

    return time < DateTime::timeInDay;
}

[[nodiscard]] auto DurationUnits::isZero() const -> bool {
    return std::ranges::all_of(units_.begin(), units_.end(),
                               [](auto value) -> bool { return value == 0; });
}

[[nodiscard]] auto DurationUnits::onlyContainsUnit(unit_t onlyUnit) const
    -> bool {
    for (auto unit{static_cast<size_t>(unit_t::YEAR)}; unit != unit_t::COUNT_;
         ++unit) {

        if (unit == static_cast<size_t>(onlyUnit) && units_[unit] == 0) {
            return false;
        }

        if (unit != static_cast<size_t>(onlyUnit) && units_[unit] != 0) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] auto DurationUnits::isMultipleOf(DurationUnits other) const
    -> bool {
    auto thisCopy{*this};

    thisCopy.convertUnitsUpwards();
    other.convertUnitsUpwards();

    for (auto unitInt{static_cast<size_t>(unit_t::YEAR)};
         unitInt != unit_t::COUNT_; ++unitInt) {
        auto unit{static_cast<unit_t>(unitInt)};

        auto thisValue{thisCopy.getUnitValue(unit)};
        auto otherValue{other.getUnitValue(unit)};

        if (otherValue == 0) {
            if (thisValue == 0) {
                continue;
            }

            return false;
        }

        if ((thisValue % otherValue) != 0) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] auto DurationUnits::isValidValue(value_t value) -> bool {
    return value <= maxValue;
}

[[nodiscard]] auto DurationUnits::operator+(const DurationUnits &other) const
    -> DurationUnits {
    auto result{DurationUnits{*this}};

    for (auto unitInt{static_cast<size_t>(unit_t::YEAR)};
         unitInt != unit_t::COUNT_; ++unitInt) {

        auto unit{static_cast<unit_t>(unitInt)};
        result.addUnit(unit, other.getUnitValue(unit));
    }

    return result;
}

[[nodiscard]] auto DurationUnits::operator<=>(const DurationUnits &other) const
    -> std::strong_ordering {

    for (auto unit{static_cast<size_t>(unit_t::YEAR)}; unit != unit_t::COUNT_;
         ++unit) {
        if (auto cmp = getUnitValue(static_cast<unit_t>(unit)) <=>
                       other.getUnitValue(static_cast<unit_t>(unit));
            cmp != 0) {
            return cmp;
        }
    }

    return std::strong_ordering::equal;
}

[[nodiscard]] auto DurationUnits::fromISO8601String(const std::string &string)
    -> std::optional<DurationUnits> {
    return util::DurationUnitsParser<util::ISO8601DurationParser>::parse(
        string);
}

[[nodiscard]] auto DurationUnits::toISO8601String() const -> std::string {
    return util::DurationUnitsParser<util::ISO8601DurationParser>::format(
        *this);
}

[[nodiscard]] auto DurationUnits::fromNaturalLanguage(const std::string &input)
    -> std::optional<DurationUnits> {
    return util::DurationUnitsParser<util::NaturalLanguageParser>::parse(input);
}

[[nodiscard]] auto DurationUnits::toNaturalLanguage() const -> std::string {
    return util::DurationUnitsParser<util::NaturalLanguageParser>::format(
        *this);
}
} // namespace hbt::mods
