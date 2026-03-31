#include <algorithm>
#include <interval.hpp>

#include <regex>
#include <string>
#include <unordered_set>

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

[[nodiscard]] auto toLower(const std::string &string) -> std::string {
    auto result{string};
    std::ranges::transform(result, result.begin(),
                           [](unsigned char c) { return std::tolower(c); });

    return result;
}

auto parseUnit(const std::string &unit, int value, DurationUnits &durationUnits)
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

    if (years.contains(u)) {
        durationUnits.addYears(value);

    } else if (months.contains(u)) {
        durationUnits.addMonths(value);

    } else if (weeks.contains(u)) {
        durationUnits.addWeeks(value);

    } else if (days.contains(u)) {
        durationUnits.addDays(value);

    } else if (hours.contains(u)) {
        durationUnits.addHours(value);

    } else if (minutes.contains(u)) {
        durationUnits.addMinutes(value);

    } else {
        return false;
    }

    return true;
}

[[nodiscard]] auto Interval::fromNaturalLanguage(const std::string &input)
    -> std::optional<Interval> {
    const std::regex pattern(R"(^(\d+\W*[a-zA-Z]+\W*){1,6}$)");
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

    for (auto it(std::sregex_iterator{filtered.begin(), filtered.end(),
                                      pairPattern});
         it != std::sregex_iterator(); ++it) {
        const auto &match{*it};

        auto value{std::stoi(match[valueGroup].str())};
        auto unit{match[unitGroup].str()};

        if (!parseUnit(unit, value, durationUnits)) {
            return std::nullopt;
        }
    }

    return Interval{durationUnits};
}
} // namespace hbt::mods
