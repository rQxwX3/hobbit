#include <date.hpp>
#include <interval.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>

namespace test::mods {
using hbt::mods::Date;
using hbt::mods::Interval;

using std::chrono::day;
using std::chrono::month;
using std::chrono::year;
using std::chrono::year_month_day;

TEST(DateTest, DefaultCtorCreatesToday) { EXPECT_EQ(Date(), Date::today()); }

TEST(DateTest, ThrowsOnInvalidValueYMD) {
    EXPECT_THROW(Date(2025, 13, 1), std::invalid_argument);

    EXPECT_THROW(Date(2025, 12, 32), std::invalid_argument);

    EXPECT_THROW(Date(2025, 13, 32), std::invalid_argument);

    EXPECT_THROW(Date(2025, -1, 1), std::invalid_argument);

    EXPECT_THROW(Date(2025, 1, -1), std::invalid_argument);

    EXPECT_THROW(Date(2025, -1, -1), std::invalid_argument);

    EXPECT_THROW(Date(2025, 0, 1), std::invalid_argument);

    EXPECT_THROW(Date(2025, 1, 0), std::invalid_argument);

    EXPECT_THROW(Date(2025, 0, 0), std::invalid_argument);

    EXPECT_THROW(Date(2025, 2, 29), std::invalid_argument); // not a leap year
    EXPECT_NO_THROW(Date(2024, 2, 29));                     // leap year

    EXPECT_THROW(Date(2025, 4, 31), std::invalid_argument);  // April
    EXPECT_THROW(Date(2025, 6, 31), std::invalid_argument);  // June
    EXPECT_THROW(Date(2025, 9, 31), std::invalid_argument);  // September
    EXPECT_THROW(Date(2025, 11, 31), std::invalid_argument); // November

    EXPECT_NO_THROW(Date(2025, 1, 31));  // January
    EXPECT_NO_THROW(Date(2025, 3, 31));  // March
    EXPECT_NO_THROW(Date(2025, 5, 31));  // May
    EXPECT_NO_THROW(Date(2025, 7, 31));  // July
    EXPECT_NO_THROW(Date(2025, 8, 31));  // August
    EXPECT_NO_THROW(Date(2025, 10, 31)); // October
    EXPECT_NO_THROW(Date(2025, 12, 31)); // December

    EXPECT_NO_THROW(Date(0, 1, 1));
    EXPECT_NO_THROW(Date(-100, 1, 1));
}

TEST(DateTest, GetWeekdayReturnsCorrectWeekday) {
    EXPECT_EQ(Date(2026, 5, 3).getWeekday(), Date::Week::SUNDAY);
    EXPECT_EQ(Date(2026, 5, 4).getWeekday(), Date::Week::MONDAY);
    EXPECT_EQ(Date(2026, 5, 5).getWeekday(), Date::Week::TUESDAY);
    EXPECT_EQ(Date(2026, 5, 6).getWeekday(), Date::Week::WEDNESDAY);
    EXPECT_EQ(Date(2026, 5, 7).getWeekday(), Date::Week::THURSDAY);
    EXPECT_EQ(Date(2026, 5, 8).getWeekday(), Date::Week::FRIDAY);
    EXPECT_EQ(Date(2026, 5, 9).getWeekday(), Date::Week::SATURDAY);
}

TEST(DateTest, IsToday) {
    EXPECT_TRUE(Date::today().isToday());

    EXPECT_FALSE(Date(2026, 4, 26).isToday());
    EXPECT_FALSE(Date(2000, 12, 31).isToday());
}

TEST(DateTest, OperatorPlusAddsMonthIntervals) {
    EXPECT_EQ(Date(2026, 5, 3) + Interval::months(1), Date(2026, 6, 3));
    EXPECT_EQ(Date(2026, 1, 30) + Interval::months(2), Date(2026, 3, 30));

    EXPECT_EQ(Date(2026, 1, 30) + Interval::months(1), Date(2026, 2, 28));
    EXPECT_EQ(Date(2025, 12, 30) + Interval::months(2), Date(2026, 2, 28));
    EXPECT_EQ(Date(2023, 1, 31) + Interval::months(1), Date(2023, 2, 28));
    EXPECT_EQ(Date(2024, 1, 31) + Interval::months(1), Date(2024, 2, 29));

    EXPECT_EQ(Date(2025, 12, 30) + Interval::months(12), Date(2026, 12, 30));
    EXPECT_EQ(Date(2025, 12, 30) + Interval::months(24), Date(2027, 12, 30));

    EXPECT_EQ(Date(2023, 2, 28) + Interval::months(1), Date(2023, 3, 28));
    EXPECT_EQ(Date(2024, 2, 29) + Interval::months(1), Date(2024, 3, 29));

    EXPECT_EQ(Date(2023, 1, 31) + Interval::months(2), Date(2023, 3, 31));

    EXPECT_EQ(Date(2023, 1, 31) + Interval::months(13), Date(2024, 2, 29));
    EXPECT_EQ(Date(2023, 3, 31) + Interval::months(11), Date(2024, 2, 29));

    EXPECT_EQ(Date(2023, 12, 31) + Interval::months(2), Date(2024, 2, 29));

    EXPECT_EQ(Date(2023, 5, 15) + Interval::months(0), Date(2023, 5, 15));
}

TEST(DateTest, OperatorPlusAddsNonMonthIntervals) {
    EXPECT_EQ(Date(2026, 1, 3) + Interval({.years = 1, .weeks = 1, .days = 1}),
              Date(2027, 1, 11));

    EXPECT_EQ(Date(2023, 2, 28) + Interval({.years = 1, .days = 1}),
              Date(2024, 2, 29));

    EXPECT_EQ(Date(2024, 2, 29) + Interval({.years = 1}), Date(2025, 2, 28));

    EXPECT_EQ(Date(2026, 1, 28) + Interval({.weeks = 1}), Date(2026, 2, 4));

    EXPECT_EQ(Date(2026, 1, 30) + Interval({.days = 5}), Date(2026, 2, 4));

    EXPECT_EQ(Date(2023, 1, 31) + Interval({.months = 1, .days = 1}),
              Date(2023, 3, 1));

    EXPECT_EQ(Date(2023, 1, 30) + Interval({.months = 1, .days = 5}),
              Date(2023, 3, 5));

    EXPECT_EQ(Date(2025, 12, 25) +
                  Interval({.years = 1, .weeks = 1, .days = 3}),
              Date(2027, 1, 4));
    EXPECT_EQ(Date(2026, 5, 10) + Interval(), Date(2026, 5, 10));

    EXPECT_EQ(Date(2020, 1, 1) +
                  Interval({.years = 10, .weeks = 52, .days = 365}),
              Date(2031, 12, 31));

    EXPECT_EQ(Date(2023, 1, 31) + Interval({.months = 1, .weeks = 1}),
              Date(2023, 3, 7));
}

TEST(DateTest, OperatorPlusAddsMixedIntervals) {
    EXPECT_EQ(Date(2026, 1, 3) + Interval({.years = 1, .weeks = 1, .days = 1}),
              Date(2027, 1, 11));

    EXPECT_EQ(Date(2023, 1, 31) + Interval({.months = 1, .days = 1}),
              Date(2023, 3, 1));

    EXPECT_EQ(Date(2023, 1, 31) + Interval({.months = 1, .weeks = 1}),
              Date(2023, 3, 7));

    EXPECT_EQ(Date(2023, 2, 28) + Interval({.years = 1, .days = 1}),
              Date(2024, 2, 29));

    EXPECT_EQ(Date(2024, 2, 29) + Interval({.years = 1, .days = 1}),
              Date(2025, 3, 1));

    EXPECT_EQ(Date(2026, 1, 28) + Interval({.weeks = 1}), Date(2026, 2, 4));

    EXPECT_EQ(Date(2026, 1, 30) + Interval({.days = 5}), Date(2026, 2, 4));

    EXPECT_EQ(Date(2025, 12, 25) +
                  Interval({.years = 1, .weeks = 1, .days = 3}),
              Date(2027, 1, 4));

    EXPECT_EQ(Date(2026, 5, 10) + Interval(), Date(2026, 5, 10));
}

TEST(DateTest, DaysBetween) {
    EXPECT_EQ(Date::daysBetween(Date::today(), Date::today()),
              Interval::days(0));

    EXPECT_EQ(Date::daysBetween(Date(2026, 5, 1), Date(2026, 5, 2)),
              Interval::days(1));

    EXPECT_EQ(Date::daysBetween(Date(2026, 5, 1), Date(2026, 5, 2)),
              Date::daysBetween(Date(2026, 5, 2), Date(2026, 5, 1)));

    EXPECT_EQ(Date::daysBetween(Date(2026, 4, 30), Date(2026, 5, 1)),
              Interval::days(1));

    EXPECT_EQ(Date::daysBetween(Date(2024, 2, 28), Date(2024, 3, 1)),
              Interval::days(2));

    EXPECT_EQ(Date::daysBetween(Date(2020, 1, 1), Date(2021, 1, 1)),
              Interval::days(366));

    auto a{Date(2023, 3, 1)};
    auto b{Date(2025, 7, 10)};
    EXPECT_EQ(Date::daysBetween(a, b), Date::daysBetween(b, a));
}
} // namespace test::mods
