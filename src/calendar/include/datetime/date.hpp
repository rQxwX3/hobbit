#pragma once

#include <chrono>
#include <cstdint>

#include <datetime/constants.hpp>
#include <datetime/interval.hpp>

namespace clndr::dt {
class Date {
  public:
    using year_t = int16_t;
    using month_t = uint8_t;
    using day_t = uint8_t;

    using duration_t = std::chrono::days;

  private:
    /* order must not be changed */
    year_t year_;
    month_t month_;
    day_t day_;

  public:
    Date();

    Date(year_t year, month_t month, day_t day);

  public:
    [[nodiscard]] auto getYear() const -> year_t;

    [[nodiscard]] auto getMonth() const -> month_t;

    [[nodiscard]] auto getDay() const -> day_t;

  public:
    auto setYear(year_t year) -> void;

    auto setMonth(month_t month) -> void;

    auto setDay(day_t day) -> void;

  public:
    [[nodiscard]] auto next(const Date &date) -> Date;

  public:
    [[nodiscard]] auto toDuration() const -> duration_t;

    [[nodiscard]] auto toYMD() const -> std::chrono::year_month_day;

    [[nodiscard]] auto getWeekday() const -> constants::Weekday;

    [[nodiscard]] auto operator<=>(const Date &other) const
        -> std::strong_ordering = default;

    [[nodiscard]] auto operator+(const Interval &interval) const -> Date;

    auto operator+=(const Interval &interval) -> Date &;
};
} // namespace clndr::dt
