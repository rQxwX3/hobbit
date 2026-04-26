#pragma once

#include <interval.hpp>

#include <chrono>

namespace hbt::mods {
class Date {
  public:
    using ymd_t = std::chrono::year_month_day;

    using year_t = std::chrono::year;
    using month_t = std::chrono::month;
    using day_t = std::chrono::day;

  public:
    enum class weekday_t : char {
        MONDAY = 0,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY,
        COUNT_,
    };

  public:
    enum class Error : uint8_t {
        InvalidYMD,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidYMD:
            return "Date: provided year-month-day value is not valid";
        }
    }

  private:
    ymd_t ymd_;

  private:
    [[nodiscard]] static auto ymdValidator(ymd_t ymd) -> ymd_t;

  public:
    Date(ymd_t ymd);

    Date(year_t year, month_t month, day_t day);

  public:
    [[nodiscard]] auto getYMD() const -> ymd_t;

    [[nodiscard]] auto getWeekday() const -> weekday_t;

    [[nodiscard]] auto getYear() const -> year_t;

    [[nodiscard]] auto getMonth() const -> month_t;

    [[nodiscard]] auto getDay() const -> day_t;

  public:
    [[nodiscard]] static auto today() -> Date;

  public:
    [[nodiscard]] auto isToday() const -> bool;

  public:
    [[nodiscard]] auto operator==(const Date &other) const -> bool;

    ;
    [[nodiscard]] auto operator<=>(const Date &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator+(const Interval &interval) const -> Date;

    [[nodiscard]] auto operator-(const Interval &interval) const -> Date;

    auto operator+=(const Interval &interval) -> Date &;

  public:
    [[nodiscard]] static auto getDiff(const Date &d1, const Date &d2)
        -> Duration;
};
} // namespace hbt::mods
