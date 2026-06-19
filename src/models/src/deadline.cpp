#include <deadline.hpp>

namespace hbt::mods {
Deadline::Deadline(underlying_type_t type)
    : type_{validateUnderlyingType(std::move(type))} {}

[[nodiscard]] auto Deadline::validateUnderlyingType(underlying_type_t type)
    -> underlying_type_t {
    if (std::holds_alternative<Interval>(type) ||
        std::holds_alternative<DateTime>(type) ||
        std::holds_alternative<std::monostate>(type)) {
        return type;
    }

    throw std::invalid_argument(errorMessage(Error::InvalidUnderlyingType));
}

[[nodiscard]] auto Deadline::null() -> Deadline { return {std::monostate()}; }

[[nodiscard]] auto Deadline::isNull() const -> bool {
    return std::holds_alternative<std::monostate>(type_);
}

[[nodiscard]] auto Deadline::isInterval() const -> bool {
    return std::holds_alternative<Interval>(type_);
}

[[nodiscard]] auto Deadline::isDateTime() const -> bool {
    return std::holds_alternative<DateTime>(type_);
}

[[nodiscard]] auto Deadline::getType() const -> Type {
    if (std::holds_alternative<Interval>(type_)) {
        return Type::Interval;
    }

    if (std::holds_alternative<DateTime>(type_)) {
        return Type::DateTime;
    }

    if (std::holds_alternative<std::monostate>(type_)) {
        return Type::Null;
    }

    throw std::runtime_error(errorMessage(Error::RTInvalidUnderlyingType));
}

[[nodiscard]] auto Deadline::getInterval() const -> Interval {
    if (getType() != Type::Interval) {
        throw std::runtime_error(errorMessage(Error::IntervalBadAccess));
    }

    return std::get<Interval>(type_);
}

[[nodiscard]] auto Deadline::getDateTime() const -> DateTime {
    if (getType() != Type::DateTime) {
        throw std::runtime_error(errorMessage(Error::DateTimeBadAccess));
    }

    return std::get<DateTime>(type_);
}

[[nodiscard]] auto Deadline::operator==(const Deadline &deadline) const
    -> bool = default;

[[nodiscard]] auto Deadline::toJSON() const -> nlohmann::json {
    if (std::holds_alternative<Interval>(type_)) {
        auto intervalJSON = std::get<Interval>(type_).toJSON();

        return {{jsonTypeField, jsonTypeIntervalValue},
                {jsonIntervalField, intervalJSON}};
    }

    if (std::holds_alternative<DateTime>(type_)) {
        auto datetimeISO8601{std::get<DateTime>(type_).toISO8601String()};
        return {{jsonTypeField, jsonTypeDateTimeValue},
                {jsonDateTimeField, datetimeISO8601}};
    }

    return {{jsonTypeField, jsonTypeNullValue}};
}

[[nodiscard]] auto Deadline::fromJSON(const nlohmann::json &json)
    -> std::expected<Deadline, Error> {
    if (!json.contains(jsonTypeField)) {
        return std::unexpected(Error::JSONMissingRequiredTypeField);
    }

    if (json[jsonTypeField] == jsonTypeIntervalValue) {
        if (!json.contains(jsonIntervalField)) {
            return std::unexpected(Error::JSONMissingRequiredIntervalField);
        }

        auto intervalFromJSON{
            hbt::mods::Interval::fromJSON(json[jsonIntervalField])};
        if (!intervalFromJSON) {
            return std::unexpected(Error::JSONFailedToParseInterval);
        }

        return Deadline{intervalFromJSON.value()};
    }

    if (json[jsonTypeField] == jsonTypeDateTimeValue) {
        if (!json.contains(jsonDateTimeField)) {
            return std::unexpected(Error::JSONMissingRequiredDateTimeField);
        }

        auto dateTimeFromJSON{
            hbt::mods::DateTime::fromISO8601String(json[jsonDateTimeField])};
        if (!dateTimeFromJSON) {
            return std::unexpected(Error::JSONFailedToParseDateTime);
        }

        return Deadline{dateTimeFromJSON.value()};
    }

    if (json[jsonTypeField] == jsonTypeNullValue) {
        return Deadline(std::monostate());
    }

    return std::unexpected(Error::JSONUnsupportedType);
}
} // namespace hbt::mods
