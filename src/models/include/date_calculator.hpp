#pragma once

#include <date.hpp>
#include <duration.hpp>

namespace hbt::mods::util {
using mods::Date;
using mods::Duration;

class DateCalculator {
  private:
    enum class months_t : YMD::month_t {
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

  private:
    class MonthDays {
      private:
        static constexpr auto array{
            std::array<Date::day_t, Duration::monthsInYear>{
                31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

      public:
        [[nodiscard]] static constexpr auto get(Date::month_t month)
            -> Date::day_t;

        [[nodiscard]] static constexpr auto get(months_t month) -> Date::day_t;
    };

    [[nodiscard]] static constexpr auto isLeapYear(Date::year_t year) -> bool;

    [[nodiscard]] static constexpr auto getMonthDays(Date::month_t month,
                                                     Date::year_t year)
        -> Date::day_t;

  public:
    using day_diff_t = size_t;

  public:
    [[nodiscard]] static auto daysBetween(const Date &first, const Date &second)
        -> day_diff_t;
    // private:
    //   using diff_t = int;
    //
    // private:
    //   struct SignedDateDiff {
    //       diff_t years{0};
    //       diff_t months{0};
    //       diff_t days{0};
    //   };
    //
    //   [[nodiscard]] static auto getSignedDateDiff(const Date &later,
    //                                               const Date &earlier)
    //       -> SignedDateDiff;
    //
    //   [[nodiscard]] static auto getBorrowMonthYear(Date::month_t
    //   currentMonth,
    //                                                Date::year_t currentYear)
    //       -> std::pair<Date::month_t, Date::year_t>;
    //
    // public:
    //   [[nodiscard]] static auto differenceCalendar(const Date &first,
    //                                                const Date &second)
    //       -> Duration;
};
} // namespace hbt::mods::util
