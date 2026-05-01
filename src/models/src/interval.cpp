#include <interval.hpp>

#include <string>

namespace hbt::mods {
Interval::Interval(hbt::mods::Duration duration, MonthHandling monthHandling)
    : duration_{duration}, monthHandling_{monthHandling} {}

Interval::Interval(const Interval &other) = default;

Interval::Interval(struct_t unitStruct, MonthHandling monthHandling)
    : duration_{Duration{unitStruct}}, monthHandling_{monthHandling} {}

[[nodiscard]] auto Interval::years(value_t value, MonthHandling monthHandling)
    -> Interval {
    return Interval{Duration::years(value), monthHandling};
}

[[nodiscard]] auto Interval::months(value_t value, MonthHandling monthHandling)
    -> Interval {
    return Interval{Duration::months(value), monthHandling};
}

[[nodiscard]] auto Interval::weeks(value_t value, MonthHandling monthHandling)
    -> Interval {
    return Interval{Duration::weeks(value), monthHandling};
}

[[nodiscard]] auto Interval::days(value_t value, MonthHandling monthHandling)
    -> Interval {
    return Interval{Duration::days(value), monthHandling};
}

[[nodiscard]] auto Interval::hours(value_t value, MonthHandling monthHandling)
    -> Interval {
    return Interval{Duration::hours(value), monthHandling};
}

[[nodiscard]] auto Interval::minutes(value_t value, MonthHandling monthHandling)
    -> Interval {
    return Interval{Duration::minutes(value), monthHandling};
}

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
    if (getMonthHandling() != other.getMonthHandling()) {
        throw std::runtime_error(errorMessage(Error::FailedToPerformAddition));
    }

    return Interval{this->duration_ + other.duration_};
}

[[nodiscard]] auto Interval::isZero() const -> bool {
    return duration_.isZero();
}

[[nodiscard]] auto Interval::onlyContainsUnit(unit_t unit) const -> bool {
    return duration_.onlyContainsUnit(unit);
}

[[nodiscard]] auto Interval::containsAllJSONFields(const nlohmann::json &json)
    -> bool {
    return std::ranges::all_of(jsonFields, [&json](const auto &field) -> bool {
        return json.contains(field);
    });
}

[[nodiscard]] auto Interval::toJSON() const -> nlohmann::json {
    return {{jsonDurationField, duration_.toISO8601String()},
            {jsonMonthHandlingField,
             std::to_string(static_cast<int>(monthHandling_))}};
}

[[nodiscard]] auto Interval::fromJSON(const nlohmann::json &json)
    -> std::expected<Interval, Error> {
    if (!containsAllJSONFields(json)) {
        return std::unexpected(Error::JSONMissingRequiredField);
    }

    auto durationFromISO8601String{hbt::mods::Duration::fromISO8601String(
        json[jsonDurationField].get<std::string>())};

    if (!durationFromISO8601String) {
        return std::unexpected(Error::JSONFailedToParseDuration);
    }

    auto monthHandling{static_cast<Interval::MonthHandling>(
        std::stoi(json[jsonMonthHandlingField].get<std::string>()))};

    return Interval{durationFromISO8601String.value(), monthHandling};
}
} // namespace hbt::mods
