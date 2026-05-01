#pragma once

#include <interval.hpp>

#include <chrono>

namespace hbt::mods {
class Date {
  public:
    using chrono_ymd_t = std::chrono::year_month_day;

    using year_t = std::chrono::year;
    using month_t = std::chrono::month;
    using day_t = std::chrono::day;

    /* types below reflect value ranges for corresponding std::chrono types */
    using year_value_t = int16_t;
    using month_value_t = uint8_t;
    using day_value_t = uint8_t;

  public:
    struct YMD {
        year_value_t year;
        month_value_t month;
        day_value_t day;
    };

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
        InvalidChronoYMD,
        InvalidYMD,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidChronoYMD:
            return "Date: provided std::chrono::year_month_day value is not "
                   "valid";

        case Error::InvalidYMD:
            return "Date: provided year-month-day value is not valid";

        default:
            return "Date: unclassified error";
        }
    }

  private:
    chrono_ymd_t chronoYMD_;

  private:
    [[nodiscard]] static auto chronoYMDValidator(chrono_ymd_t chronoYMD)
        -> chrono_ymd_t;

    [[nodiscard]] static auto ymdValidator(YMD ymd) -> chrono_ymd_t;

  public:
    Date(chrono_ymd_t ymd);

    Date(year_t year, month_t month, day_t day);

    Date(YMD ymd);

  public:
    [[nodiscard]] auto getChronoYMD() const -> chrono_ymd_t;

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

    [[nodiscard]] auto operator!=(const Date &other) const -> bool;

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
