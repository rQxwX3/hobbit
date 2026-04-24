#include <datetime.hpp>
#include <duration.hpp>
#include <duration_parser.hpp>

#include <algorithm>
#include <string>

namespace hbt::mods {
auto Duration::validateValue(value_t value) -> value_t {
    if (value > maxValue) {
        throw std::invalid_argument(errorMessage(Error::InvalidValue));
    }

    return value;
}

auto Duration::validateArray(array_t array) -> array_t {
    for (auto value : array) {
        try {
            validateValue(value);
        } catch (std::invalid_argument) {
            throw std::invalid_argument(errorMessage(Error::InvalidArray));
        }
    }

    return array;
}

[[nodiscard]] auto Duration::validateStruct(struct_t unitsStruct) -> struct_t {
    try {
        validateArray(unitsStruct.toArray());
    } catch (std::invalid_argument) {
        throw std::invalid_argument(errorMessage(Error::InvalidStruct));
    }

    return unitsStruct;
}

Duration::Duration() : units_{array_t{}} {}

Duration::Duration(array_t unitsArray) : units_{validateArray(unitsArray)} {}

Duration::Duration(const Units &unitsStruct)
    : units_{validateStruct(unitsStruct).toArray()} {};

auto Duration::convertUnitsUpwards() -> Duration {
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

[[nodiscard]] auto Duration::fromUnit(unit_t unit, value_t value) -> Duration {
    auto array{array_t{}};
    array[unit] = validateValue(value);

    return Duration{array}.convertUnitsUpwards();
}

[[nodiscard]] auto Duration::years(value_t value) -> Duration {
    return fromUnit(unit_t::YEAR, value);
}

[[nodiscard]] auto Duration::months(value_t value) -> Duration {
    return fromUnit(unit_t::MONTH, value);
}

[[nodiscard]] auto Duration::weeks(value_t value) -> Duration {
    return fromUnit(unit_t::WEEK, value);
}

[[nodiscard]] auto Duration::days(value_t value) -> Duration {
    return fromUnit(unit_t::DAY, value);
}

[[nodiscard]] auto Duration::hours(value_t value) -> Duration {
    return fromUnit(unit_t::HOUR, value);
}

[[nodiscard]] auto Duration::minutes(value_t value) -> Duration {
    return fromUnit(unit_t::MINUTE, value);
}

[[nodiscard]] auto Duration::getMaxNonZeroUnit() const
    -> std::optional<unit_t> {
    for (auto unit{static_cast<size_t>(unit_t::YEAR)}; unit != unit_t::COUNT_;
         ++unit) {
        if (units_[unit] != 0) {
            return static_cast<unit_t>(unit);
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto Duration::getNonZeroUnitValuePairs() const
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

auto Duration::addUnit(unit_t unit, value_t value) -> void {
    auto result{units_[unit] + value};

    try {
        units_[unit] = validateValue(result);
    } catch (std::invalid_argument) {
        units_[unit] = result % maxValue;
    }
}

[[nodiscard]] auto Duration::getUnitValue(unit_t unit) const -> value_t {
    return units_[unit];
}

[[nodiscard]] auto Duration::isZero() const -> bool {
    return std::ranges::all_of(units_.begin(), units_.end(),
                               [](auto value) -> bool { return value == 0; });
}

[[nodiscard]] auto Duration::onlyContainsUnit(unit_t onlyUnit) const -> bool {
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

[[nodiscard]] auto Duration::isMultipleOf(Duration other) const -> bool {
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

[[nodiscard]] auto Duration::operator+(const Duration &other) const
    -> Duration {
    auto result{Duration{*this}};

    for (auto unitInt{static_cast<size_t>(unit_t::YEAR)};
         unitInt != unit_t::COUNT_; ++unitInt) {

        auto unit{static_cast<unit_t>(unitInt)};
        result.addUnit(unit, other.getUnitValue(unit));
    }

    return result;
}

[[nodiscard]] auto Duration::operator<=>(const Duration &other) const
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

[[nodiscard]] auto Duration::fromISO8601String(const std::string &string)
    -> std::expected<Duration, Error> {
    auto duration{
        util::DurationParser<util::ISO8601DurationParser>::parse(string)};

    if (!duration) {
        return std::unexpected(Error::ISO8601FailedToParse);
    }

    return duration.value();
}

[[nodiscard]] auto Duration::toISO8601String() const -> std::string {
    return util::DurationParser<util::ISO8601DurationParser>::format(*this);
}

[[nodiscard]] auto Duration::fromNaturalLanguage(const std::string &input)
    -> std::expected<Duration, Error> {
    auto duration{
        util::DurationParser<util::NaturalLanguageParser>::parse(input)};

    if (!duration) {
        return std::unexpected(Error::NaturalLanguageFailedToParse);
    }

    return duration.value();
}

[[nodiscard]] auto Duration::toNaturalLanguage() const -> std::string {
    return util::DurationParser<util::NaturalLanguageParser>::format(*this);
}
} // namespace hbt::mods
