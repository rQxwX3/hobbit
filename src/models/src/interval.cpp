#include <datetime.hpp>
#include <interval.hpp>
#include <interval_parser.hpp>

#include <algorithm>
#include <string>

namespace hbt::mods {
Interval::Interval(MonthHandling monthHandling)
    : units_{array_t{}}, monthHandling_{monthHandling} {}

Interval::Interval(array_t unitsArray, MonthHandling monthHandling)
    : units_{unitsArray}, monthHandling_{monthHandling} {}

Interval::Interval(const struct_t &unitsStruct, MonthHandling monthHandling)
    : units_{unitsStruct.toArray()}, monthHandling_{monthHandling} {};

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
    array[unit] = value;

    return Interval(array);
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
    units_[unit] += value;
}

[[nodiscard]] auto Interval::getUnitsStruct() const -> struct_t {
    return struct_t::fromArray(units_);
}

[[nodiscard]] auto Interval::isZero() const -> bool {
    return std::ranges::all_of(units_.begin(), units_.end(),
                               [](auto value) -> bool { return value == 0; });
}

[[nodiscard]] auto Interval::onlyContainsUnit(Unit onlyUnit) const -> bool {
    for (const auto unit : Interval::units) {
        if (unit == onlyUnit && units_[unit] == 0) {
            return false;
        }

        if (unit != onlyUnit && units_[unit] != 0) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] auto Interval::isMultipleOf(Interval other) const -> bool {
    if (isZero()) {
        return false;
    }

    auto convertedThis{this->convertUnitsDownwards()};
    auto convertedOther{other.convertUnitsDownwards()};

    auto isUnitMultipleOf([&convertedThis, &convertedOther](Unit unit) -> bool {
        auto thisValue{convertedThis[unit]};
        auto otherValue{convertedOther[unit]};

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
        result.addUnit(unit, other[unit]);
    }

    return result;
}

[[nodiscard]] auto Interval::operator<=>(const Interval &other) const
    -> std::strong_ordering {
    auto lhs{this->convertUnitsDownwards()};
    auto rhs{other.convertUnitsDownwards()};

    /* months are not convertible to minutes and vice versa */
    const bool convertedDownToTheSameUnit{
        (!lhs[Unit::MONTH] && !rhs[Unit::MONTH]) ||
        (!lhs[Unit::MINUTE] && !rhs[Unit::MINUTE])};

    if (!convertedDownToTheSameUnit) {
        throw std::logic_error(errorMessage(Error::InvalidComparison));
    }

    if (auto cmp{lhs[Unit::MONTH] <=> rhs[Unit::MONTH]};
        cmp != std::strong_ordering::equal) {
        return cmp;
    }

    if (auto cmp{lhs[Unit::MINUTE] <=> rhs[Unit::MINUTE]};
        cmp != std::strong_ordering::equal) {
        return cmp;
    }

    return std::strong_ordering::equal;
}

[[nodiscard]] auto Interval::operator==(const Interval &other) const -> bool {
    auto lhs{this->convertUnitsDownwards()};
    auto rhs{other.convertUnitsDownwards()};

    /* only minutes and months are left after the conversion */
    const auto minutesAreEqual{lhs[Unit::MINUTE] == rhs[Unit::MINUTE]};
    const auto monthsAreEqual{lhs[Unit::MONTH] == rhs[Unit::MONTH]};

    return minutesAreEqual && monthsAreEqual;
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
