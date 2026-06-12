#pragma once

#include <bitset>
#include <expected>
#include <string>

#include <nlohmann/json.hpp>

namespace hbt::mods {
class Week {
  public:
    enum class Weekday : uint8_t {
        SUNDAY,
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        COUNT_,
    };

  public:
    static constexpr size_t weekdaysCount{static_cast<size_t>(Weekday::COUNT_)};

    static constexpr auto weekdays{std::array<Weekday, Week::weekdaysCount>{
        {Weekday::SUNDAY, Weekday::MONDAY, Weekday::TUESDAY, Weekday::WEDNESDAY,
         Weekday::THURSDAY, Weekday::FRIDAY, Weekday::SATURDAY}}};

  public:
    enum class Error : uint8_t {
        JSONNotString,
        JSONInvalidString,

        StringLengthExceedsLimit,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONNotString:
            return "Week: provided JSON doesn't contains a string";

        case Error::JSONInvalidString:
            return "Week: provided JSON contains invalid string";

        case Error::StringLengthExceedsLimit:
            return "Week: provided string exceeds maximum length";

        default:
            std::unreachable();
        }
    }

  public:
    using days_t = std::bitset<weekdaysCount>;

  private:
    days_t days_;

  public:
    [[nodiscard]] auto operator==(const Week &) const -> bool = default;

  public:
    Week(days_t days = days_t{});

    Week(const std::vector<Week::Weekday> &weekdays);

  public:
    [[nodiscard]] auto getDays() const -> days_t;

    [[nodiscard]] auto containsWeekday(Week::Weekday weekday) const -> bool;

    [[nodiscard]] auto isEmpty() const -> bool;

  private:
    static auto validateAndReverseString(std::string string) -> std::string;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<Week, Error>;
};
} // namespace hbt::mods
