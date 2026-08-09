#include <datetime/datetime.hpp>
#include <datetime/error/interval.hpp>
#include <datetime/interval.hpp>
#include <datetime/parsing/interval.hpp>
#include <datetime/schema/interval.hpp>

#include <algorithm>
#include <string>

namespace clndr::dt {
Interval::Interval() = default;

Interval::Interval(MonthHandling monthHandling)
    : array_{array_t{}}, monthHandling_{monthHandling} {
    schema::interval::Schema::validateAllRules(*this);
}

Interval::Interval(array_t unitsArray, MonthHandling monthHandling)
    : array_{unitsArray}, monthHandling_{monthHandling} {
    schema::interval::Schema::validateAllRules(*this);
}

Interval::Interval(const struct_t &unitsStruct, MonthHandling monthHandling)
    : array_{unitsStruct.toArray()}, monthHandling_{monthHandling} {
    schema::interval::Schema::validateAllRules(*this);
};

[[nodiscard]] auto Interval::getArray() const -> array_t { return array_; }

[[nodiscard]] auto Interval::getMonthHandling() const -> MonthHandling {
    return monthHandling_;
}

auto Interval::setArray(array_t array) -> void {
    array_ = array;

    schema::interval::Schema::validateAffectedRules<
        schema::interval::fields::Array>(*this);
}

auto Interval::setMonthHandling(MonthHandling monthHandling) -> void {
    monthHandling_ = monthHandling;

    schema::interval::Schema::validateAffectedRules<
        schema::interval::fields::MonthHandling>(*this);
}

[[nodiscard]] auto Interval::convertUnitsDownwards() const -> Interval {
    auto copy{*this};

    auto convertDownwards{
        [&copy](Unit from, Unit to, value_t conversionRatio) -> void {
            assert(from < to);
            copy.array_[to] += copy.array_[from] * conversionRatio;
            copy.array_[from] = 0;
        }};

    convertDownwards(Unit::YEAR, Unit::MONTH, dt::constants::monthsInYear);
    convertDownwards(Unit::WEEK, Unit::DAY, dt::constants::daysInWeek);
    convertDownwards(Unit::DAY, Unit::HOUR, dt::constants::hoursInDay);
    convertDownwards(Unit::HOUR, Unit::MINUTE, dt::constants::minutesInHour);

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
        if (auto value{array_[unit]}; value) {
            result.emplace_back(unit, value);
        }
    }

    return result;
}

auto Interval::addUnit(Unit unit, value_t value) -> void {
    array_[unit] += value;
}

[[nodiscard]] auto Interval::getUnitsStruct() const -> struct_t {
    return struct_t::fromArray(array_);
}

[[nodiscard]] auto Interval::isZero() const -> bool {
    return std::ranges::all_of(array_.begin(), array_.end(),
                               [](auto value) -> bool { return value == 0; });
}

[[nodiscard]] auto Interval::onlyContainsUnit(Unit onlyUnit) const -> bool {
    for (const auto unit : Interval::units) {
        if (unit == onlyUnit && array_[unit] == 0) {
            return false;
        }

        if (unit != onlyUnit && array_[unit] != 0) {
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
    const bool lhsIsMixed{lhs[Unit::MINUTE] != 0 && lhs[Unit::MONTH] != 0};
    const bool rhsIsMixed{rhs[Unit::MINUTE] != 0 && rhs[Unit::MONTH] != 0};

    if (lhsIsMixed || rhsIsMixed) {
        throw std::logic_error(
            std::string(error::interval::InvalidComparison::msg));
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
    return array_[unit];
}

[[nodiscard]] auto Interval::fromNaturalLanguage(const std::string &input)
    -> std::expected<Interval, core::error::code_t> {
    auto interval{
        dt::parsing::IntervalParser<dt::parsing::NaturalLanguageParser>::parse(
            input)};

    if (!interval) {
        return std::unexpected(
            error::interval::NaturalLanguageFailedToParse::code);
    }

    return interval.value();
}

[[nodiscard]] auto Interval::toNaturalLanguage() const -> std::string {
    return dt::parsing::IntervalParser<
        dt::parsing::NaturalLanguageParser>::format(*this);
}
} // namespace clndr::dt
