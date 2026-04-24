#pragma once

#include <datetime.hpp>
#include <interval.hpp>

#include <nlohmann/json.hpp>

#include <expected>
#include <string>
#include <variant>

namespace hbt::mods {
class Deadline {
  public:
    enum class Error : uint8_t {
        JSONMissingRequiredTypeField,

        JSONMissingRequiredIntervalField,
        JSONFailedToParseInterval,

        JSONMissingRequiredDateTimeField,
        JSONFailedToParseDateTime,

        JSONUnsupportedType,
    };

    using type_t = std::variant<hbt::mods::Interval, hbt::mods::DateTime>;

  private:
    static constexpr auto jsonTypeField{std::string_view{"type"}};

    static constexpr auto jsonIntervalField{std::string_view{"interval"}};
    static constexpr auto jsonDateTimeField{std::string_view{"datetime"}};

    static constexpr auto jsonTypeIntervalValue{std::string_view{"interval"}};
    static constexpr auto jsonTypeDateTimeValue{std::string_view{"datetime"}};

  private:
    type_t type_;

  public:
    Deadline(type_t type);

  public:
    [[nodiscard]] auto getUnderlyingType() const -> type_t;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<Deadline, Error>;
};
} // namespace hbt::mods
