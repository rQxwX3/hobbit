#include <deadline.hpp>

namespace hbt::mods {
Deadline::Deadline(type_t type) : type_{std::move(type)} {}

[[nodiscard]] auto Deadline::getUnderlyingType() const -> type_t {
    return std::visit([](const auto &value) -> type_t { return value; }, type_);
}

[[nodiscard]] auto Deadline::toJSON() const -> nlohmann::json {
    if (std::holds_alternative<Interval>(type_)) {
        auto intervalJSON{std::get<Interval>(type_).toJSON()};

        return {{jsonTypeField, jsonTypeIntervalValue},
                {jsonIntervalField, intervalJSON}};
    }

    auto datetimeISO8601{std::get<DateTime>(type_).toISO8601String()};

    return {{jsonTypeField, jsonTypeDateTimeValue},
            {jsonDateTimeField, datetimeISO8601}};
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

    return std::unexpected(Error::JSONUnsupportedType);
}
} // namespace hbt::mods
