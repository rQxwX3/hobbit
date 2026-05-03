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

    using value_t = int16_t;

  public:
    enum class Week : uint8_t {
        SUNDAY,
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
    };

    enum class Month : uint8_t {
        JANUARY = 1,
        FEBRUARY,
        MARCH,
        APRIL,
        MAY,
        JUNE,
        JULY,
        AUGUST,
        SEPTEMBER,
        OCTOBER,
        NOVEMBER,
        DECEMBER,
    };

    using weekday_t = Week;

  public:
    static constexpr auto daysInWeek{value_t{7}};
    static constexpr auto monthsInYear{value_t{12}};

    static constexpr auto monthDays{std::array<value_t, monthsInYear>{
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

    [[nodiscard]] static auto getDaysInMonth(value_t month, value_t year)
        -> value_t {
        if (month < 1 || monthsInYear < month) {
            throw std::runtime_error(errorMessage(Error::InvalidMonth));
        }

        const bool isLeapYear{(year % 4 == 0 && year % 100 != 0) ||
                              (year % 400 == 0)};

        auto daysInMonth{monthDays[month - 1]};

        if (month == static_cast<value_t>(Month::FEBRUARY)) {
            return (isLeapYear) ? daysInMonth + 1 : daysInMonth;
        }

        return daysInMonth;
    }

  public:
    enum class Error : uint8_t {
        InvalidYear,
        InvalidMonth,
        InvalidDay,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidYear:
            return "Date: provided year value is not valid";

        case Error::InvalidMonth:
            return "Date: provided month value is not valid";

        case Error::InvalidDay:
            return "Date: provided day value is not valid";

        default:
            std::unreachable();
        }
    }

  private:
    ymd_t ymd_;

  private:
    [[nodiscard]] static auto valueYMDValidator(value_t year, value_t month,
                                                value_t day) -> ymd_t;

  public:
    Date();

    Date(ymd_t ymd);

    Date(year_t year, month_t month, day_t day);

    Date(value_t year, value_t month, value_t day);

  public:
    [[nodiscard]] auto getYMD() const -> ymd_t;

    [[nodiscard]] auto getYear() const -> year_t;

    [[nodiscard]] auto getMonth() const -> month_t;

    [[nodiscard]] auto getDay() const -> day_t;

    [[nodiscard]] auto getWeekday() const -> weekday_t;

  public:
    [[nodiscard]] static auto today() -> Date;

  public:
    [[nodiscard]] auto isToday() const -> bool;

    [[nodiscard]] auto ok() const -> bool;

  public:
    [[nodiscard]] auto operator<=>(const Date &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const Date &other) const -> bool;

    [[nodiscard]] auto operator!=(const Date &other) const -> bool;

    [[nodiscard]] auto operator+(const Interval &interval) const -> Date;

    auto operator+=(const Interval &interval) -> Date &;

  public:
    [[nodiscard]] static auto daysBetween(const Date &first, const Date &second)
        -> Interval;
};
} // namespace hbt::mods
