#pragma once

#include <datetime.hpp>
#include <interval.hpp>

#include <nlohmann/json.hpp>

#include <expected>
#include <string_view>
#include <variant>

namespace hbt::mods {
class Deadline {
  public:
    enum class Type : uint8_t {
        Interval,
        DateTime,
    };

    using type_t = std::variant<hbt::mods::Interval, hbt::mods::DateTime>;

  public:
    enum class Error : uint8_t {
        JSONMissingRequiredTypeField,

        JSONMissingRequiredIntervalField,
        JSONFailedToParseInterval,

        JSONMissingRequiredDateTimeField,
        JSONFailedToParseDateTime,

        JSONUnsupportedType,

        InvalidUnderlyingType,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredTypeField:
            return "Deadline: missing required Type field in JSON";

        case Error::JSONMissingRequiredIntervalField:
            return "Deadline: missing required Interval field in JSON";

        case Error::JSONFailedToParseInterval:
            return "Deadline: failed to parse Interval from JSON";

        case Error::JSONMissingRequiredDateTimeField:
            return "Deadline: missing required DateTime field in JSON";

        case Error::JSONFailedToParseDateTime:
            return "Deadline: failed to parse DateTime from JSON";

        case Error::JSONUnsupportedType:
            return "Deadline: unsupported Type provided in JSON";

        case Error::InvalidUnderlyingType:
            return "Deadline: provided value is not of a valid deadline type";
        }
    }

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

  private:
    [[nodiscard]] auto validateUnderlyingType(type_t type) const -> type_t;

  public:
    [[nodiscard]] auto getType() const -> Type;

    [[nodiscard]] auto getInterval() const -> Interval;

    [[nodiscard]] auto getDateTime() const -> DateTime;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<Deadline, Error>;
};
} // namespace hbt::mods
