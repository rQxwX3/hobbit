#include <deadline.hpp>

namespace hbt::mods {
Deadline::Deadline(type_t type)
    : type_{validateUnderlyingType(std::move(type))} {}

[[nodiscard]] auto Deadline::validateUnderlyingType(type_t type) -> type_t {
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
    return std::get<Interval>(type_);
}

[[nodiscard]] auto Deadline::getDateTime() const -> DateTime {
    return std::get<DateTime>(type_);
}

[[nodiscard]] auto Deadline::toJSON() const -> nlohmann::json {
    if (std::holds_alternative<Interval>(type_)) {
        auto intervalJSON{std::get<Interval>(type_).toISO8601String()};

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

        auto intervalFromISO8601{hbt::mods::Interval::fromISO8601String(
            json[jsonIntervalField].get<std::string>())};
        if (!intervalFromISO8601) {
            return std::unexpected(Error::JSONFailedToParseInterval);
        }

        return Deadline{intervalFromISO8601.value()};
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
