#pragma once

#include <nlohmann/json.hpp>

#include <interval.hpp>

#include <chrono>

namespace hbt::mods {
class Date {
  public:
    using weekday_t = std::chrono::weekday;

  private:
    std::chrono::year_month_day ymd_;

  public:
    Date();

    explicit Date(std::chrono::year_month_day ymd);

    explicit Date(std::chrono::year year, std::chrono::month month,
                  std::chrono::day day);

  public:
    [[nodiscard]] static auto today() -> Date;

  public:
    [[nodiscard]] auto getYear() const -> std::chrono::year;

    [[nodiscard]] auto getMonth() const -> std::chrono::month;

    [[nodiscard]] auto getDay() const -> std::chrono::day;

    [[nodiscard]] auto getYMD() const -> std::chrono::year_month_day;

    [[nodiscard]] auto getWeekday() const -> weekday_t;

  public:
    [[nodiscard]] auto operator<=>(const Date &other) const
        -> std::strong_ordering = default;

    [[nodiscard]] auto operator==(const Date &other) const -> bool = default;

  public:
    [[nodiscard]] auto isToday() const -> bool;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::optional<Date>;

  public:
    [[nodiscard]] auto operator+(Interval interval) const -> Date;

    auto operator+=(Interval interval) -> Date &;
};
} // namespace hbt::mods
