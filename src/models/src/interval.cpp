#include <interval.hpp>

#include <string>

namespace hbt::mods {
Interval::Interval(hbt::mods::Duration duration, MonthHandling monthHandling)
    : duration_{duration}, monthHandling_{monthHandling} {}

Interval::Interval(const Interval &other) = default;

Interval::Interval(hbt::mods::Duration::Units units,
                   MonthHandling monthHandling)
    : duration_{hbt::mods::Duration{units}}, monthHandling_{monthHandling} {}

[[nodiscard]] auto Interval::getUnitValue(unit_t unit) const -> value_t {
    return duration_.getUnitValue(unit);
}

[[nodiscard]] auto Interval::getMonthHandling() const -> MonthHandling {
    return monthHandling_;
}

[[nodiscard]] auto Interval::getDuration() const -> Duration {
    return duration_;
}

auto Interval::setMonthHandling(MonthHandling monthHandling) -> void {
    monthHandling_ = monthHandling;
}

[[nodiscard]] auto Interval::operator+(const Interval &other) const
    -> Interval {
    return Interval{this->duration_ + other.duration_};
}

[[nodiscard]] auto Interval::isZero() const -> bool {
    return duration_.isZero();
}

[[nodiscard]] auto Interval::onlyContainsUnit(unit_t unit) const -> bool {
    return duration_.onlyContainsUnit(unit);
}

[[nodiscard]] auto Interval::toJSON() const -> nlohmann::json {
    return {
        {"duration", duration_.toISO8601String()},
        {"month_handling", std::to_string(static_cast<int>(monthHandling_))}};
}

[[nodiscard]] auto Interval::fromJSON(const nlohmann::json &json)
    -> std::optional<Interval> {
    if (!json.contains("duration") || !json.contains("month_handling")) {
        return std::nullopt;
    }

    auto durationFromISO8601String{hbt::mods::Duration::fromISO8601String(
        json["duration"].get<std::string>())};

    if (!durationFromISO8601String.has_value()) {
        return std::nullopt;
    }

    auto monthHandling{static_cast<Interval::MonthHandling>(
        std::stoi(json["month_handling"].get<std::string>()))};

    return Interval{durationFromISO8601String.value(), monthHandling};
}

[[nodiscard]] auto Interval::fromNaturalLanguage(const std::string &input)
    -> std::optional<Interval> {
    auto duration{Duration::fromNaturalLanguage(input)};

    if (duration.has_value()) {
        return Interval{duration.value()};
    }

    return std::nullopt;
}

[[nodiscard]] auto Interval::toNaturalLanguage() const -> std::string {
    return duration_.toNaturalLanguage();
}
} // namespace hbt::mods
