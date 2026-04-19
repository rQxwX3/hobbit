#include <duration_units.hpp>
#include <duration_units_parser.hpp>

#include <algorithm>
#include <string>

namespace hbt::mods {
DurationUnits::DurationUnits() : units_{array_t{}} {}

DurationUnits::DurationUnits(array_t unitsArray) : units_{unitsArray} {}

DurationUnits::DurationUnits(const Units &unitsStruct)
    : units_{unitsStruct.toArray()} {};

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

[[nodiscard]] auto DurationUnits::getUnit(unit_t unit) const -> value_t {
    return units_[unit];
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

[[nodiscard]] auto DurationUnits::isValidValue(value_t value) -> bool {
    return value <= maxValue;
}

[[nodiscard]] auto DurationUnits::operator+(const DurationUnits &other) const
    -> DurationUnits {
    auto result{DurationUnits{*this}};

    for (auto unitInt{static_cast<size_t>(unit_t::YEAR)};
         unitInt != unit_t::COUNT_; ++unitInt) {

        auto unit{static_cast<unit_t>(unitInt)};
        result.addUnit(unit, other.getUnit(unit));
    }

    return result;
}

[[nodiscard]] auto DurationUnits::operator<=>(const DurationUnits &other) const
    -> std::strong_ordering {

    for (auto unit{static_cast<size_t>(unit_t::YEAR)}; unit != unit_t::COUNT_;
         ++unit) {
        if (auto cmp = getUnit(static_cast<unit_t>(unit)) <=>
                       other.getUnit(static_cast<unit_t>(unit));
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
