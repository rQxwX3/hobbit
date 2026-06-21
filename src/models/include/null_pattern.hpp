#pragma once

#include <recurrence_pattern.hpp>

#include <nlohmann/json.hpp>

namespace hbt::mods::util {
class NullRecurrencePattern : public RecurrencePattern {
  public:
    NullRecurrencePattern() = default;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime on) const
        -> occurrences_t override;

  public:
    struct JSON {
        enum class Error : uint8_t {
            FailedToParse,
        };

        static constexpr auto value{std::string_view("null_pattern")};

        [[nodiscard]] static auto encode() -> nlohmann::json;

        [[nodiscard]] static auto decode(const nlohmann::json &json)
            -> std::expected<NullRecurrencePattern, Error>;
    };
};
} // namespace hbt::mods::util
