#include <datetime.hpp>
#include <interval.hpp>
#include <interval_parser.hpp>

#include <algorithm>
#include <string>

namespace hbt::mods {
auto Interval::validateValue(value_t value) -> value_t {
    // if (value > maxValue) {
    //     throw std::invalid_argument(errorMessage(Error::InvalidValue));
    // }

    return value;
}

[[nodiscard]] auto Interval::isValidValue(value_t value) -> bool {
    try {
        Interval::validateValue(value);

        return true;
    } catch (std::invalid_argument) {
        return false;
    }
}

auto Interval::validateArray(array_t array) -> array_t {
    for (auto value : array) {
        try {
            validateValue(value);
        } catch (std::invalid_argument) {
            throw std::invalid_argument(errorMessage(Error::InvalidArray));
        }
    }

    return array;
}

[[nodiscard]] auto Interval::validateStruct(struct_t unitsStruct) -> struct_t {
    try {
        validateArray(unitsStruct.toArray());
    } catch (std::invalid_argument) {
        throw std::invalid_argument(errorMessage(Error::InvalidStruct));
    }

    return unitsStruct;
}

Interval::Interval() : units_{array_t{}} {}

Interval::Interval(array_t unitsArray) : units_{validateArray(unitsArray)} {}

Interval::Interval(const Units &unitsStruct)
    : units_{validateStruct(unitsStruct).toArray()} {};

[[nodiscard]] auto Interval::convertUnitsUpwards() const -> Interval {
    auto copy{*this};

    auto convertUpwards{
        [&copy](unit_t from, unit_t to, value_t conversionRatio) -> void {
            assert(to < from);

            copy.units_[to] += copy.units_[from] / conversionRatio;
            copy.units_[from] %= conversionRatio;
        }};

    convertUpwards(unit_t::MINUTE, unit_t::HOUR, minutesInHour);
    convertUpwards(unit_t::HOUR, unit_t::DAY, hoursInDay);
    convertUpwards(unit_t::DAY, unit_t::WEEK, daysInWeek);
    convertUpwards(unit_t::MONTH, unit_t::YEAR, monthsInYear);

    return copy;
}

[[nodiscard]] auto Interval::convertUnitsDownwards() const -> Interval {
    auto copy{*this};

    auto convertDownwards{
        [&copy](unit_t from, unit_t to, value_t conversionRatio) -> void {
            assert(from < to);
            copy.units_[to] += copy.units_[from] * conversionRatio;
            copy.units_[from] = 0;
        }};

    convertDownwards(unit_t::YEAR, unit_t::MONTH, monthsInYear);
    convertDownwards(unit_t::WEEK, unit_t::DAY, daysInWeek);
    convertDownwards(unit_t::DAY, unit_t::HOUR, hoursInDay);
    convertDownwards(unit_t::HOUR, unit_t::MINUTE, minutesInHour);

    return copy;
}

[[nodiscard]] auto Interval::fromUnit(unit_t unit, value_t value) -> Interval {
    auto array{array_t{}};
    array[unit] = validateValue(value);

    return Interval{array};
}

[[nodiscard]] auto Interval::years(value_t value) -> Interval {
    return fromUnit(unit_t::YEAR, value);
}

[[nodiscard]] auto Interval::months(value_t value) -> Interval {
    return fromUnit(unit_t::MONTH, value);
}

[[nodiscard]] auto Interval::weeks(value_t value) -> Interval {
    return fromUnit(unit_t::WEEK, value);
}

[[nodiscard]] auto Interval::days(value_t value) -> Interval {
    return fromUnit(unit_t::DAY, value);
}

[[nodiscard]] auto Interval::hours(value_t value) -> Interval {
    return fromUnit(unit_t::HOUR, value);
}

[[nodiscard]] auto Interval::minutes(value_t value) -> Interval {
    return fromUnit(unit_t::MINUTE, value);
}

// NOT USED
// [[nodiscard]] auto Interval::getMaxNonZeroUnit() const
//     -> std::optional<unit_t> {
//     auto result{std::optional<unit_t>(std::nullopt)};
//
//     for (const auto unit : Interval::units) {
//         if (!result && units_[unit] != 0) {
//             result = unit;
//         }
//     }
//
//     return result;
// }

[[nodiscard]] auto Interval::getNonZeroUnitValuePairs() const
    -> std::vector<unitValuePair_t> {
    std::vector<unitValuePair_t> result;
    result.reserve(unit_t::COUNT_);

    for (const auto unit : Interval::units) {
        if (auto value{units_[unit]}; value) {
            result.emplace_back(unit, value);
        }
    }

    return result;
}

auto Interval::addUnit(unit_t unit, value_t value) -> void {
    auto result{units_[unit] + value};

    try {
        units_[unit] = validateValue(result);
    } catch (std::invalid_argument) {
        units_[unit] = result % maxValue;
    }
}

[[nodiscard]] auto Interval::getUnitValue(unit_t unit) const -> value_t {
    return units_[unit];
}

[[nodiscard]] auto Interval::getUnits() const -> Units {
    return Units::fromArray(units_);
}

[[nodiscard]] auto Interval::isZero() const -> bool {
    return std::ranges::all_of(units_.begin(), units_.end(),
                               [](auto value) -> bool { return value == 0; });
}

[[nodiscard]] auto Interval::onlyContainsUnit(unit_t onlyUnit) const -> bool {
    for (const auto unit : Interval::units) {
        if (unit == static_cast<size_t>(onlyUnit) && units_[unit] == 0) {
            return false;
        }

        if (unit != static_cast<size_t>(onlyUnit) && units_[unit] != 0) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] auto Interval::isMultipleOf(Interval other) const -> bool {
    auto convertedThis{this->convertUnitsDownwards()};
    auto convertedOther{other.convertUnitsDownwards()};

    auto isUnitMultipleOf(
        [&convertedThis, &convertedOther](unit_t unit) -> bool {
            auto thisValue{convertedThis.getUnitValue(unit)};
            auto otherValue{convertedOther.getUnitValue(unit)};

            if (otherValue == 0) {
                return thisValue == 0;
            }

            return thisValue % otherValue == 0;
        });

    return isUnitMultipleOf(unit_t::MONTH) && isUnitMultipleOf(unit_t::MINUTE);
}

[[nodiscard]] auto Interval::operator+(const Interval &other) const
    -> Interval {
    auto result{Interval{*this}};

    for (const auto unit : Interval::units) {
        result.addUnit(unit, other.getUnitValue(unit));
    }

    return result;
}

[[nodiscard]] auto Interval::operator<=>(const Interval &other) const
    -> std::strong_ordering {
    auto convertedThis{this->convertUnitsUpwards()};
    auto convertedOther{other.convertUnitsUpwards()};

    for (const auto unit : Interval::units) {
        if (auto cmp{convertedThis.getUnitValue(unit) <=>
                     convertedOther.getUnitValue(unit)};
            cmp != 0) {
            return cmp;
        }
    }

    return std::strong_ordering::equal;
}

[[nodiscard]] auto Interval::operator==(const Interval &other) const -> bool {
    auto convertedThis(this->convertUnitsDownwards());
    auto convertedOther(other.convertUnitsDownwards());

    for (const auto unit : Interval::units) {
        if (convertedOther.units_[unit] != convertedThis.units_[unit]) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] auto Interval::operator[](unit_t unit) const -> value_t {
    return units_[unit];
}

[[nodiscard]] auto Interval::fromISO8601String(const std::string &string)
    -> std::expected<Interval, Error> {
    auto interval{
        util::IntervalParser<util::ISO8601IntervalParser>::parse(string)};

    if (!interval) {
        return std::unexpected(Error::ISO8601FailedToParse);
    }

    return interval.value();
}

[[nodiscard]] auto Interval::toISO8601String() const -> std::string {
    return util::IntervalParser<util::ISO8601IntervalParser>::format(*this);
}

[[nodiscard]] auto Interval::fromNaturalLanguage(const std::string &input)
    -> std::expected<Interval, Error> {
    auto interval{
        util::IntervalParser<util::NaturalLanguageParser>::parse(input)};

    if (!interval) {
        return std::unexpected(Error::NaturalLanguageFailedToParse);
    }

    return interval.value();
}

[[nodiscard]] auto Interval::toNaturalLanguage() const -> std::string {
    return util::IntervalParser<util::NaturalLanguageParser>::format(*this);
}
} // namespace hbt::mods
