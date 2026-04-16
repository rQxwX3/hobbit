#pragma once

#include <date.hpp>

#include <bitset>

namespace hbt::mods {
class Week {
  private:
    using days_t = std::bitset<static_cast<size_t>(Date::weekday_t::COUNT_)>;

  private:
    days_t days_;

  public:
    [[nodiscard]] auto operator==(const Week &) const -> bool = default;

  public:
    Week(const std::vector<Date::weekday_t> &weekdays);

    Week(const std::string &daysString);

  public:
    [[nodiscard]] auto getDays() const -> days_t;
};
} // namespace hbt::mods
