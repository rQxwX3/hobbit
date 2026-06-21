#pragma once

#include <optional>

#include <datetime.hpp>

namespace hbt::mods::util {
class OptDateTime {
  private:
    std::optional<DateTime> optDateTime_;

  public:
    OptDateTime(std::optional<DateTime> optDateTime);

  public:
    [[nodiscard]] auto operator==(const OptDateTime &other) const
        -> bool = default;

  public:
    [[nodiscard]] auto hasValue() const -> bool;

    [[nodiscard]] auto getValue() const -> DateTime;

  public:
    struct JSON {
        enum class Error : uint8_t {
            FailedToParseDateTime,
        };

        static constexpr auto nullValue{std::string_view("none")};

        static auto encode(const OptDateTime &optDateTime) -> nlohmann::json;

        static auto decode(const nlohmann::json &json)
            -> std::expected<OptDateTime, Error>;
    };
};
} // namespace hbt::mods::util
