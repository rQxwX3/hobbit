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

Interval::Interval(MonthHandling monthHandling)
    : units_{array_t{}}, monthHandling_{monthHandling} {}

Interval::Interval(array_t unitsArray, MonthHandling monthHandling)
    : units_{validateArray(unitsArray)}, monthHandling_{monthHandling} {}

Interval::Interval(const Units &unitsStruct, MonthHandling monthHandling)
    : units_{validateStruct(unitsStruct).toArray()},
      monthHandling_{monthHandling} {};

[[nodiscard]] auto Interval::convertUnitsUpwards() const -> Interval {
    auto copy{*this};

    auto convertUpwards{
        [&copy](Unit from, Unit to, value_t conversionRatio) -> void {
            assert(to < from);

            copy.units_[to] += copy.units_[from] / conversionRatio;
            copy.units_[from] %= conversionRatio;
        }};

    convertUpwards(Unit::MINUTE, Unit::HOUR, minutesInHour);
    convertUpwards(Unit::HOUR, Unit::DAY, hoursInDay);
    convertUpwards(Unit::DAY, Unit::WEEK, daysInWeek);
    convertUpwards(Unit::MONTH, Unit::YEAR, monthsInYear);

    return copy;
}

[[nodiscard]] auto Interval::convertUnitsDownwards() const -> Interval {
    auto copy{*this};

    auto convertDownwards{
        [&copy](Unit from, Unit to, value_t conversionRatio) -> void {
            assert(from < to);
            copy.units_[to] += copy.units_[from] * conversionRatio;
            copy.units_[from] = 0;
        }};

    convertDownwards(Unit::YEAR, Unit::MONTH, monthsInYear);
    convertDownwards(Unit::WEEK, Unit::DAY, daysInWeek);
    convertDownwards(Unit::DAY, Unit::HOUR, hoursInDay);
    convertDownwards(Unit::HOUR, Unit::MINUTE, minutesInHour);

    return copy;
}

[[nodiscard]] auto Interval::fromUnit(Unit unit, value_t value) -> Interval {
    auto array{array_t{}};
    array[unit] = validateValue(value);

    return Interval{array};
}

[[nodiscard]] auto Interval::years(value_t value) -> Interval {
    return fromUnit(Unit::YEAR, value);
}

[[nodiscard]] auto Interval::months(value_t value) -> Interval {
    return fromUnit(Unit::MONTH, value);
}

[[nodiscard]] auto Interval::weeks(value_t value) -> Interval {
    return fromUnit(Unit::WEEK, value);
}

[[nodiscard]] auto Interval::days(value_t value) -> Interval {
    return fromUnit(Unit::DAY, value);
}

[[nodiscard]] auto Interval::hours(value_t value) -> Interval {
    return fromUnit(Unit::HOUR, value);
}

[[nodiscard]] auto Interval::minutes(value_t value) -> Interval {
    return fromUnit(Unit::MINUTE, value);
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
    result.reserve(Unit::COUNT_);

    for (const auto unit : Interval::units) {
        if (auto value{units_[unit]}; value) {
            result.emplace_back(unit, value);
        }
    }

    return result;
}

[[nodiscard]] auto Interval::getMonthHandling() const -> MonthHandling {
    return monthHandling_;
}

auto Interval::addUnit(Unit unit, value_t value) -> void {
    auto result{units_[unit] + value};

    try {
        units_[unit] = validateValue(result);
    } catch (std::invalid_argument) {
        units_[unit] = result % maxValue;
    }
}

[[nodiscard]] auto Interval::getUnitValue(Unit unit) const -> value_t {
    return units_[unit];
}

[[nodiscard]] auto Interval::getUnits() const -> Units {
    return Units::fromArray(units_);
}

[[nodiscard]] auto Interval::isZero() const -> bool {
    return std::ranges::all_of(units_.begin(), units_.end(),
                               [](auto value) -> bool { return value == 0; });
}

[[nodiscard]] auto Interval::onlyContainsUnit(Unit onlyUnit) const -> bool {
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

    auto isUnitMultipleOf([&convertedThis, &convertedOther](Unit unit) -> bool {
        auto thisValue{convertedThis.getUnitValue(unit)};
        auto otherValue{convertedOther.getUnitValue(unit)};

        if (otherValue == 0) {
            return thisValue == 0;
        }

        return thisValue % otherValue == 0;
    });

    return isUnitMultipleOf(Unit::MONTH) && isUnitMultipleOf(Unit::MINUTE);
}

[[nodiscard]] auto Interval::operator+(const Interval &other) const
    -> Interval {
    auto result{Interval{*this}}; // preserves lhs' MonthHandling

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

[[nodiscard]] auto Interval::operator[](Unit unit) const -> value_t {
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
