#include <duration_units.hpp>
#include <duration_units_parser.hpp>

#include <algorithm>
#include <regex>
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

    for (size_t i{0}; i != unit_t::COUNT_; ++i) {
        auto value{units_[i]};

        if (value == 0) {
            continue;
        }

        result.emplace_back(static_cast<unit_t>(i), value);
    }

    return result;
}

// auto DurationUnits::addYears(value_t value) -> void {
//     units_[unit_t::YEAR] += value;
// }
//
// auto DurationUnits::addMonths(value_t value) -> void {
//     this->addYears(value / DurationUnits::monthsInYear);
//
//     units_[unit_t::MONTH] += value % DurationUnits::monthsInYear;
// }
//
// auto DurationUnits::addWeeks(value_t value) -> void {
// There is 4.35 weeks in a month -> unit conversion is not accurate

//     units_[unit_t::WEEK] += value;
// }
//
// auto DurationUnits::addDays(value_t value) -> void {
//     this->addWeeks(value / DurationUnits::daysInWeek);
//
//     units_[unit_t::DAY] += value % DurationUnits::daysInWeek;
// }
//
// auto DurationUnits::addHours(value_t value) -> void {
//     this->addDays(value / DurationUnits::hoursInDay);
//
//     units_[unit_t::HOUR] += value % DurationUnits::hoursInDay;
// }
//
// auto DurationUnits::addMinutes(value_t value) -> void {
//     this->addHours(value / DurationUnits::minutesInHour);
//
//     units_[unit_t::MINUTE] += value % DurationUnits::minutesInHour;
// }

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

[[nodiscard]] auto DurationUnits::onlyContainsUnit(unit_t unit) const -> bool {
    for (size_t currUnit{0}; currUnit != unit_t::COUNT_; ++currUnit) {
        if (currUnit == static_cast<size_t>(unit) && units_[currUnit] == 0) {
            return false;
        }

        if (currUnit != static_cast<size_t>(unit) && units_[currUnit] != 0) {
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

    for (size_t unitInt{unit_t::YEAR}; unitInt != unit_t::COUNT_; ++unitInt) {
        auto unit{static_cast<unit_t>(unitInt)};

        result.addUnit(unit, other.getUnit(unit));
    }

    return result;
}

[[nodiscard]] auto DurationUnits::operator<=>(const DurationUnits &other) const
    -> std::strong_ordering {
    if (auto cmp = getUnit(unit_t::YEAR) <=> other.getUnit(unit_t::YEAR);
        cmp != 0) {
        return cmp;
    }

    if (auto cmp = getUnit(unit_t::MONTH) <=> other.getUnit(unit_t::MONTH);
        cmp != 0) {
        return cmp;
    }

    if (auto cmp = getUnit(unit_t::WEEK) <=> other.getUnit(unit_t::WEEK);
        cmp != 0) {
        return cmp;
    }

    if (auto cmp = getUnit(unit_t::DAY) <=> other.getUnit(unit_t::DAY);
        cmp != 0) {
        return cmp;
    }

    if (auto cmp = getUnit(unit_t::HOUR) <=> other.getUnit(unit_t::HOUR);
        cmp != 0) {
        return cmp;
    }

    if (auto cmp = getUnit(unit_t::MINUTE) <=> other.getUnit(unit_t::MINUTE);
        cmp != 0) {
        return cmp;
    }

    return std::strong_ordering::equal;
}

[[nodiscard]] auto DurationUnits::toISO8601String() const -> std::string {
    if (this->isZero()) {
        return "PT0M";
    }

    std::string result{"P"};
    bool timeSectionStarted{false};

    if (const auto years{getUnit(unit_t::YEAR)}; years) {
        result += std::to_string(years);
        result += "Y";
    }

    if (const auto months{getUnit(unit_t::MONTH)}; months) {
        result += std::to_string(months);
        result += "M";
    }

    if (const auto weeks{getUnit(unit_t::WEEK)}; weeks) {
        result += std::to_string(weeks);
        result += "W";
    }

    if (const auto days{getUnit(unit_t::DAY)}; days) {
        result += std::to_string(days);
        result += "D";
    }

    if (const auto hours{getUnit(unit_t::HOUR)}; hours) {
        if (!timeSectionStarted) {
            result += "T";
            timeSectionStarted = true;
        }

        result += std::to_string(hours);
        result += "H";
    }

    if (const auto minutes{getUnit(unit_t::MINUTE)}; minutes) {
        if (!timeSectionStarted) {
            result += "T";
            timeSectionStarted = true;
        }

        result += std::to_string(minutes);
        result += "M";
    }

    return result;
}

[[nodiscard]] auto DurationUnits::fromISO8601String(const std::string &string)
    -> std::optional<DurationUnits> {
    /*
     * ISO8601 Duration regex pattern adapted from:
     * https://stackoverflow.com/a/32045167
     * (modified to exclude seconds group)
     */
    const auto pattern{std::regex{
        R"(^P(?!$)(\d+(?:\.\d+)?Y)?(\d+(?:\.\d+)?M)?(\d+(?:\.\d+)?W)?(\d+(?:\.\d+)?D)?(T(?=\d)(\d+(?:\.\d+)?H)?(\d+(?:\.\d+)?M)?)?$)"}};

    constexpr size_t yearsGroup{1};
    constexpr size_t monthsGroup{2};
    constexpr size_t weeksGroup{3};
    constexpr size_t daysGroup{4};
    constexpr size_t timeSectionGroup{5};
    constexpr size_t hoursGroup{6};
    constexpr size_t minutesGroup{7};

    std::smatch matches;
    if (!std::regex_match(string, matches, pattern)) {
        return std::nullopt;
    }

    DurationUnits result;

    if (matches[yearsGroup].matched) {
        auto str{matches[yearsGroup].str()};
        str.pop_back(); // Remove trailing 'Y'
        result.addUnit(unit_t::YEAR, static_cast<long long>(std::stod(str)));
    }

    if (matches[monthsGroup].matched) {
        auto str{matches[monthsGroup].str()};
        str.pop_back(); // Remove trailing 'M'
        result.addUnit(unit_t::MONTH, static_cast<long long>(std::stod(str)));
    }

    if (matches[weeksGroup].matched) {
        auto str{matches[weeksGroup].str()};
        str.pop_back(); // Remove trailing 'W'
        result.addUnit(unit_t::WEEK, static_cast<long long>(std::stod(str)));
    }

    if (matches[daysGroup].matched) {
        auto str{matches[daysGroup].str()};
        str.pop_back(); // Remove trailing 'D'
        result.addUnit(unit_t::DAY, static_cast<long long>(std::stod(str)));
    }

    if (matches[hoursGroup].matched) {
        auto str{matches[hoursGroup].str()};
        str.pop_back(); // Remove trailing 'H'
        result.addUnit(unit_t::HOUR, static_cast<long long>(std::stod(str)));
    }

    if (matches[minutesGroup].matched) {
        auto str{matches[minutesGroup].str()};
        str.pop_back(); // Remove trailing 'M'
        result.addUnit(unit_t::MINUTE, static_cast<long long>(std::stod(str)));
    }

    return result;
}

[[nodiscard]] auto DurationUnits::fromNaturalLanguage(const std::string &input)
    -> std::optional<DurationUnits> {
    return util::DurationUnitsParser::fromNaturalLanguage(input);
}

[[nodiscard]] auto DurationUnits::toNaturalLanguage() const -> std::string {
    return util::DurationUnitsParser::toNaturalLanguage(*this);
}
} // namespace hbt::mods
