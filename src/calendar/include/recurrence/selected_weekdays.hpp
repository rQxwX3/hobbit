#pragma once

#include <bitset>

#include <datetime/week.hpp>

#include <nlohmann/json.hpp>

namespace clndr::rec {
class SelectedWeekdays {
  public:
    using days_t = std::bitset<dt::constants::weekdaysCount>;

  private:
    days_t days_;

  public:
    SelectedWeekdays(days_t days = days_t{});

    SelectedWeekdays(const std::vector<dt::constants::Weekday> &weekdays);

  public:
    [[nodiscard]] auto operator==(const SelectedWeekdays &) const
        -> bool = default;

  public:
    [[nodiscard]] auto getDays() const -> days_t;

    [[nodiscard]] auto contains(dt::constants::Weekday wd) const -> bool;

    [[nodiscard]] auto isEmpty() const -> bool;

  public:
    auto add(dt::constants::Weekday wd) -> void;
};
} // namespace clndr::rec
