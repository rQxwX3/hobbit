#include <null_pattern.hpp>

namespace hbt::mods::util {
[[nodiscard]] auto
NullRecurrencePattern::getOccurrencesOfDate(Date date, DateTime start) const
    -> occurrences_t {
    if (happensOnDate(date, start)) {
        return {date};
    }

    return {};
}

[[nodiscard]] auto NullRecurrencePattern::happensOnDate(Date date,
                                                        DateTime start) const
    -> bool {
    return date == start;
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
