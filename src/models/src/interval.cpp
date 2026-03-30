#include <interval.hpp>

#include <algorithm>
#include <regex>
#include <string>

namespace hbt::mods {
Interval::Interval(hbt::mods::DurationUnits durationUnits,
                   MonthHandling monthHandling)
    : durationUnits_{durationUnits}, monthHandling_{monthHandling} {}

Interval::Interval(hbt::mods::DurationUnits::Units units,
                   MonthHandling monthHandling)
    : durationUnits_{hbt::mods::DurationUnits{units}},
      monthHandling_{monthHandling} {}

[[nodiscard]] auto Interval::years(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addYears(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::months(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addMonths(value);

    return Interval{durationUnits, MonthHandling::CUT_OFF};
}

[[nodiscard]] auto Interval::weeks(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addWeeks(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::days(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addDays(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::hours(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addHours(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::minutes(value_t value) -> Interval {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addMinutes(value);

    return Interval{durationUnits};
}

[[nodiscard]] auto Interval::getYears() const -> value_t {
    return durationUnits_.getYears();
}

[[nodiscard]] auto Interval::getMonths() const -> value_t {
    return durationUnits_.getMonths();
}

[[nodiscard]] auto Interval::getWeeks() const -> value_t {
    return durationUnits_.getWeeks();
}

[[nodiscard]] auto Interval::getDays() const -> value_t {
    return durationUnits_.getDays();
}

[[nodiscard]] auto Interval::getHours() const -> value_t {
    return durationUnits_.getHours();
}

[[nodiscard]] auto Interval::getMinutes() const -> value_t {
    return durationUnits_.getMinutes();
}

[[nodiscard]] auto Interval::getMonthHandling() const -> MonthHandling {
    return monthHandling_;
}

auto Interval::setMonthHandling(MonthHandling monthHandling) -> void {
    monthHandling_ = monthHandling;
}

[[nodiscard]] auto Interval::operator+(const Interval &other) const
    -> Interval {
    return Interval{this->durationUnits_ + other.durationUnits_};
}

[[nodiscard]] auto Interval::isZero() const -> bool {
    return durationUnits_.isZero();
}
[[nodiscard]] auto Interval::toJSON() const -> nlohmann::json {
    return {
        {"duration_units", durationUnits_.toISO8601String()},
        {"month_handling", std::to_string(static_cast<int>(monthHandling_))}};
}

[[nodiscard]] auto Interval::fromJSON(const nlohmann::json &json)
    -> std::optional<Interval> {
    if (!json.contains("duration_units") || !json.contains("month_handling")) {
        return std::nullopt;
    }

    auto durationUnitsFromISO8601String{
        hbt::mods::DurationUnits::fromISO8601String(
            json["duration_units"].get<std::string>())};

    if (!durationUnitsFromISO8601String.has_value()) {
        return std::nullopt;
    }

    auto monthHandling{static_cast<Interval::MonthHandling>(
        std::stoi(json["month_handling"].get<std::string>()))};

    return Interval{durationUnitsFromISO8601String.value(), monthHandling};
}

[[nodiscard]] auto Interval::fromNaturalLanguage(const std::string &input)
    -> std::optional<Interval> {
    if (input.empty()) {
        return std::nullopt;
    }

    /*
     * Prevents replacing separators with whitespaces in inputs like 1.5 years
     * (which would lead to 1 5 years)
     */
    auto invalidSeparatorsPattern(std::regex(R"(\d+[^a-zA-Z0-9\s]\d+)"));
    if (std::regex_search(input, invalidSeparatorsPattern)) {
        return std::nullopt;
    }

    const auto pattern{std::regex(
        R"(^(?:(\d+)\s*y(?:ears?)?\s*)?(?:(\d+)\s*mo(?:nths?)?\s*)?(?:(\d+)\s*w(?:eeks?)?\s*)?(?:(\d+)\s*d(?:ays?)?\s*)?(?:(\d+)\s*h(?:ours?)?\s*)?(?:(\d+)\s*m(?:in(?:utes?)?)?\s*)?$)",
        std::regex::icase)};

    constexpr size_t yearsGroup{1};
    constexpr size_t monthsGroup{2};
    constexpr size_t weeksGroup{3};
    constexpr size_t daysGroup{4};
    constexpr size_t hoursGroup{5};
    constexpr size_t minutesGroup{6};

    std::string cleaned;
    std::replace_copy_if(
        input.begin(), input.end(), std::back_inserter(cleaned),
        [](char c) -> bool { return !std::isalnum(c) && !std::isspace(c); },
        ' ');

    std::smatch matches;
    if (!std::regex_match(cleaned, matches, pattern)) {
        return std::nullopt;
    }

    auto durationUnits{hbt::mods::DurationUnits{}};

    if (matches[yearsGroup].matched) {
        durationUnits.addYears(std::stoi(matches[yearsGroup]));
    }

    if (matches[monthsGroup].matched) {
        durationUnits.addMonths(std::stoi(matches[monthsGroup]));
    }

    if (matches[weeksGroup].matched) {
        durationUnits.addWeeks(std::stoi(matches[weeksGroup]));
    }

    if (matches[daysGroup].matched) {
        durationUnits.addDays(std::stoi(matches[daysGroup]));
    }

    if (matches[hoursGroup].matched) {
        durationUnits.addHours(std::stoi(matches[hoursGroup]));
    }

    if (matches[minutesGroup].matched) {
        durationUnits.addMinutes(std::stoi(matches[minutesGroup]));
    }

    return Interval{durationUnits};
}
} // namespace hbt::mods
