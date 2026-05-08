#pragma once

#include <bitset>
#include <expected>
#include <string>

#include <nlohmann/json.hpp>

namespace hbt::mods {
class Weekdays {
  public:
    enum class Week : uint8_t {
        SUNDAY,
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        COUNT_,
    };

  private:
    enum class Error : uint8_t {
        JSONNotString,
        JSONInvalidString,

        StringLengthMoreThanWeek,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONNotString:
            return "Weekdays: provided JSON doesn't contains a string";

        case Error::JSONInvalidString:
            return "Weekdays: provided JSON contains invalid string";

        case Error::StringLengthMoreThanWeek:
            return "Weekdays: provided string exceeds maximum length";

        default:
            std::unreachable();
        }
    }

  public:
    static constexpr size_t daysCount{static_cast<size_t>(Week::COUNT_)};

  public:
    using days_t = std::bitset<daysCount>;

  private:
    days_t days_;

  public:
    [[nodiscard]] auto operator==(const Weekdays &) const -> bool = default;

  public:
    Weekdays(const std::vector<Week> &weekdays);

    Weekdays(days_t days);

  public:
    [[nodiscard]] auto getDays() const -> days_t;

    [[nodiscard]] auto containsWeekday(Week weekday) const -> bool;

  private:
    static auto validateAndReverseString(std::string string) -> std::string;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<Weekdays, Error>;
};
} // namespace hbt::mods
