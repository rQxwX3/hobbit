#include <opt_datetime.hpp>

namespace hbt::mods::util {
OptDateTime::OptDateTime(std::optional<DateTime> optDateTime)
    : optDateTime_{optDateTime} {}

[[nodiscard]] auto OptDateTime::hasValue() const -> bool {
    return optDateTime_.has_value();
}

[[nodiscard]] auto OptDateTime::getValue() const -> DateTime {
    return *optDateTime_;
}

auto OptDateTime::JSON::encode(const OptDateTime &optDateTime)
    -> nlohmann::json {
    if (optDateTime.hasValue()) {
        return optDateTime.getValue().toISO8601String();
    }

    return nullValue;
}

auto OptDateTime::JSON::decode(const nlohmann::json &json)
    -> std::expected<OptDateTime, Error> {
    const auto optDateTimeJSON{json.get<std::string>()};

    if (optDateTimeJSON == nullValue) {
        return OptDateTime(std::nullopt);
    }

    auto dateTimeFromJSON{DateTime::fromISO8601String(optDateTimeJSON)};
    if (!dateTimeFromJSON) {
        return std::unexpected(Error::FailedToParseDateTime);
    }

    return OptDateTime(dateTimeFromJSON.value());
}
}; // namespace hbt::mods::util
