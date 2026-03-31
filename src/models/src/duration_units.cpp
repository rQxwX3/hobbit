#include <duration_units.hpp>

#include <algorithm>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <unordered_set>

namespace hbt::mods {
DurationUnits::DurationUnits() : units_{array_t{}} {}

DurationUnits::DurationUnits(array_t unitsArray) : units_{unitsArray} {}

DurationUnits::DurationUnits(const Units &unitsStruct)
    : units_{unitsStruct.toArray()} {};

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

[[nodiscard]] auto DurationUnits::isZero() const -> bool {
    return std::ranges::all_of(units_.begin(), units_.end(),
                               [](auto value) -> bool { return value == 0; });
}

[[nodiscard]] auto DurationUnits::operator+(const DurationUnits &other) const
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

[[nodiscard]] auto DurationUnits::operator<=>(const DurationUnits &other) const
    -> std::strong_ordering {
    if (auto cmp = getYears() <=> other.getYears(); cmp != 0) {
        return cmp;
    }

    if (auto cmp = getMonths() <=> other.getMonths(); cmp != 0) {
        return cmp;
    }

    if (auto cmp = getWeeks() <=> other.getWeeks(); cmp != 0) {
        return cmp;
    }

    if (auto cmp = getDays() <=> other.getDays(); cmp != 0) {
        return cmp;
    }

    if (auto cmp = getHours() <=> other.getHours(); cmp != 0) {
        return cmp;
    }

    if (auto cmp = getMinutes() <=> other.getMinutes(); cmp != 0) {
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

    if (const auto years{getYears()}; years) {
        result += std::to_string(years);
        result += "Y";
    }

    if (const auto months{getMonths()}; months) {
        result += std::to_string(months);
        result += "M";
    }

    if (const auto weeks{getWeeks()}; weeks) {
        result += std::to_string(weeks);
        result += "W";
    }

    if (const auto days{getDays()}; days) {
        result += std::to_string(days);
        result += "D";
    }

    if (const auto hours{getHours()}; hours) {
        if (!timeSectionStarted) {
            result += "T";
            timeSectionStarted = true;
        }

        result += std::to_string(hours);
        result += "H";
    }

    if (const auto minutes{getMinutes()}; minutes) {
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
        result.addYears(static_cast<long long>(std::stod(str)));
    }

    if (matches[monthsGroup].matched) {
        auto str{matches[monthsGroup].str()};
        str.pop_back(); // Remove trailing 'M'
        result.addMonths(static_cast<long long>(std::stod(str)));
    }

    if (matches[weeksGroup].matched) {
        auto str{matches[weeksGroup].str()};
        str.pop_back(); // Remove trailing 'W'
        result.addWeeks(static_cast<long long>(std::stod(str)));
    }

    if (matches[daysGroup].matched) {
        auto str{matches[daysGroup].str()};
        str.pop_back(); // Remove trailing 'D'
        result.addDays(static_cast<long long>(std::stod(str)));
    }

    if (matches[hoursGroup].matched) {
        auto str{matches[hoursGroup].str()};
        str.pop_back(); // Remove trailing 'H'
        result.addHours(static_cast<long long>(std::stod(str)));
    }

    if (matches[minutesGroup].matched) {
        auto str{matches[minutesGroup].str()};
        str.pop_back(); // Remove trailing 'M'
        result.addMinutes(static_cast<long long>(std::stod(str)));
    }

    return result;
}

[[nodiscard]] auto toLower(const std::string &string) -> std::string {
    auto result{string};
    std::ranges::transform(result, result.begin(),
                           [](unsigned char c) { return std::tolower(c); });

    return result;
}

auto parseUnit(const std::string &unit, int value, DurationUnits &durationUnits,
               std::array<bool, DurationUnits::unit_t::COUNT_> &foundUnits)
    -> bool {
    const auto u{toLower(unit)};

    static const std::unordered_set<std::string> years{"y", "yr", "yrs", "year",
                                                       "years"};
    static const std::unordered_set<std::string> months{"mo", "month",
                                                        "months"};
    static const std::unordered_set<std::string> weeks{"w", "wk", "wks", "week",
                                                       "weeks"};
    static const std::unordered_set<std::string> days{"d", "day", "days"};
    static const std::unordered_set<std::string> hours{"h", "hr", "hrs", "hour",
                                                       "hours"};
    static const std::unordered_set<std::string> minutes{"m", "min", "mins",
                                                         "minute", "minutes"};

    auto markUnitAsFound{[&foundUnits](DurationUnits::unit_t unit) -> bool {
        if (foundUnits[unit]) {
            return false;
        }

        foundUnits[unit] = true;
        return true;
    }};

    if (years.contains(u)) {
        if (!markUnitAsFound(DurationUnits::unit_t::YEAR)) {
            return false;
        }

        durationUnits.addYears(value);

    } else if (months.contains(u)) {
        if (!markUnitAsFound(DurationUnits::unit_t::MONTH)) {
            return false;
        }

        durationUnits.addMonths(value);

    } else if (weeks.contains(u)) {
        if (!markUnitAsFound(DurationUnits::unit_t::WEEK)) {
            return false;
        }

        durationUnits.addWeeks(value);

    } else if (days.contains(u)) {
        if (!markUnitAsFound(DurationUnits::unit_t::DAY)) {
            return false;
        }

        durationUnits.addDays(value);

    } else if (hours.contains(u)) {
        if (!markUnitAsFound(DurationUnits::unit_t::HOUR)) {
            return false;
        }

        durationUnits.addHours(value);

    } else if (minutes.contains(u)) {
        if (!markUnitAsFound(DurationUnits::unit_t::MINUTE)) {
            return false;
        }

        durationUnits.addMinutes(value);

    } else {
        return false;
    }

    return true;
}

[[nodiscard]] auto DurationUnits::fromNaturalLanguage(const std::string &input)
    -> std::optional<DurationUnits> {
    const std::regex pattern(R"(^([\W_]*\d+[\W_]*[a-zA-Z]+[\W_]*){1,6}$)");
    if (!std::regex_match(input, pattern)) {
        return std::nullopt;
    }

    std::string filtered;
    std::ranges::copy(input | std::ranges::views::filter([](unsigned char c) {
                          return std::isalnum(c);
                      }),
                      std::back_inserter(filtered));

    const std::regex pairPattern(R"((\d+)([A-Za-z]+))");
    constexpr size_t valueGroup{1};
    constexpr size_t unitGroup{2};

    auto durationUnits{DurationUnits{}};
    auto foundUnits{std::array<bool, DurationUnits::unit_t::COUNT_>{false}};

    for (auto it(std::sregex_iterator{filtered.begin(), filtered.end(),
                                      pairPattern});
         it != std::sregex_iterator(); ++it) {
        const auto &match{*it};

        auto value{std::stoi(match[valueGroup].str())};
        auto unit{match[unitGroup].str()};

        if (!parseUnit(unit, value, durationUnits, foundUnits)) {
            return std::nullopt;
        }
    }

    return durationUnits;
}
} // namespace hbt::mods
