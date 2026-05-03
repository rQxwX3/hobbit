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

    /* https://en.cppreference.com/cpp/chrono/year */
    static constexpr value_t minYearValue{-32767};
    static constexpr value_t maxYearValue{32767};

    /* https://en.cppreference.com/cpp/chrono/month */
    static constexpr value_t minMonthValue{1};
    static constexpr value_t maxMonthValue{12};

    /* https://en.cppreference.com/cpp/chrono/day */
    static constexpr value_t minDayValue{1};
    static constexpr value_t maxDayValue{31};

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
    static constexpr auto monthDays{std::array<value_t, Interval::monthsInYear>{
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

    [[nodiscard]] static auto getDaysInMonth(value_t month, value_t year)
        -> value_t {
        if (month < 1 || Interval::monthsInYear < month) {
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
