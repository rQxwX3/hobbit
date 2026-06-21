#include <null_pattern.hpp>

namespace hbt::mods::util {
[[nodiscard]] auto
NullRecurrencePattern::getOccurrencesOfDate(DateTime on) const
    -> occurrences_t {
    if (on == dateTime_) {
        return {on};
    }

    return {};
}

[[nodiscard]] auto NullRecurrencePattern::JSON::encode() -> nlohmann::json {
    return value;
}

[[nodiscard]] auto
NullRecurrencePattern::JSON::decode(const nlohmann::json &json)
    -> std::expected<NullRecurrencePattern, Error> {
    if (json.get<std::string>() != value) {
        return std::unexpected(Error::FailedToParse);
    }

    return NullRecurrencePattern();
}
} // namespace hbt::mods::util
