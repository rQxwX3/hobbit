#pragma once

#include <array>
#include <expected>

#include <nlohmann/json.hpp>

#include <datetime.hpp>

namespace hbt::mods::clndr {
class Week {
  public:
    using array_t = std::array<mods::DateTime, mods::Week::weekdaysCount>;

  public:
    enum class Error : uint8_t {
        JSONFailedtoParseDateTime,
    };

    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONFailedtoParseDateTime:
            return "clndr::Week: failed to parse DateTime from JSON";

        default:
            std::unreachable();
        }
    }

  private:
    array_t array_;

  public:
    Week(mods::DateTime dt);

    Week(array_t array);

  public:
    [[nodiscard]] auto operator[](mods::Week::Weekday wd) -> DateTime &;

    [[nodiscard]] auto operator[](mods::Week::Weekday wd) const -> DateTime;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<Week, Error>;
};
}; // namespace hbt::mods::clndr
