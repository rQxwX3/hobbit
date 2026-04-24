#pragma once

#include <datetime.hpp>

#include <bitset>
#include <expected>

namespace hbt::mods {
class Weekdays {
  private:
    enum class Error : uint8_t {
        JSONNotString,
        StringLengthMoreThanWeek,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONNotString:
            return "Weekdays: provided JSON doesn't contains a string";

        case Error::StringLengthMoreThanWeek:
            return "Weekdays: provided string exceeds maximum length";
        }
    }

  public:
    static constexpr size_t daysCount{
        static_cast<size_t>(DateTime::weekday_t::COUNT_)};

  public:
    using days_t = std::bitset<daysCount>;
    using weekday_t = DateTime::weekday_t;

  private:
    days_t days_;

  public:
    [[nodiscard]] auto operator==(const Weekdays &) const -> bool = default;

  public:
    Weekdays(const std::vector<DateTime::weekday_t> &weekdays);

    Weekdays(days_t days);

    Weekdays(const std::string &daysString);

  public:
    [[nodiscard]] auto getDays() const -> days_t;

    [[nodiscard]] auto containsWeekday(weekday_t weekday) const -> bool;

  private:
    static auto validateString(const std::string &string) -> std::string;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<Weekdays, Error>;
};
} // namespace hbt::mods
