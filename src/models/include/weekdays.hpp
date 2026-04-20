#pragma once

#include <datetime.hpp>

#include <bitset>

namespace hbt::mods {
class Weekdays {
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

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<Weekdays>;
};
} // namespace hbt::mods
