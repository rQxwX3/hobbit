#include <duration_units.hpp>

#include <algorithm>

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

[[nodiscard]] auto DurationUnits::toISO8601String() -> std::string {
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

    if (const auto minutes{durationUnits_.getMinutes()}; minutes) {
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
    const std::regex pattern(R"(^P(?=.)"
                             "(?:(\\d+)Y)?"
                             "(?:(\\d+)M)?"
                             "(?:(\\d+)W)?"
                             "(?:(\\d+)D)?"
                             "(?:T(?="
                             "(?:(\\d+)H)?"
                             "(?:(\\d+)M)?"
                             "))?"
                             "$)");

    constexpr size_t yearsGroup{1};
    constexpr size_t monthsGroup{2};
    constexpr size_t weeksGroup{3};
    constexpr size_t daysGroup{4};
    constexpr size_t hoursGroup{5};
    constexpr size_t minutesGroup{5};

    std::smatch matches;
    if (!std::regex_match(string, matches, pattern)) {
        return std::nullopt;
    }

    DurationUnits result;

    if (matches[yearsGroup].matched) {
        result.addYears(std::stoll(matches[yearsGroup].str()));
    }

    if (matches[monthsGroup].matched) {
        result.addMonths(std::stoll(matches[monthsGroup].str()));
    }

    if (matches[weeksGroup].matched) {
        result.addWeeks(std::stoll(matches[weeksGroup].str()));
    }

    if (matches[daysGroup].matched) {
        result.addDays(std::stoll(matches[daysGroup].str()));
    }

    if (matches[hoursGroup].matched) {
        result.addHours(std::stoll(matches[hoursGroup].str()));
    }

    if (matches[minutesGroup].matched) {
        result.addMinutes(std::stoll(matches[minutesGroup].str()));
    }

    return result;
}
} // namespace hbt::mods::util
