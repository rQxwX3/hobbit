#include <date.hpp>
#include <interval.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>

namespace test::mods {
using hbt::mods::Date;
using std::chrono::day;
using std::chrono::month;
using std::chrono::year;
using std::chrono::year_month_day;

TEST(DateTest, IsToday) {
    EXPECT_TRUE(Date::today().isToday());

    EXPECT_FALSE(Date(2026, 4, 26).isToday());

    EXPECT_FALSE(Date(2000, 12, 31).isToday());
}

TEST(DateTest, OperatorPlusAddsMonthIntervals) {
    auto date{Date(2026, 1, 30)}; // not a leap year

    auto clampInterval{hbt::mods::Interval(
        {.months = 1}, hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampInterval, Date(2026, 2, 28));

    auto resolveInterval{hbt::mods::Interval(
        {.months = 1}, hbt::mods::Interval::MonthHandling::RESOLVE_OVERFLOW)};
    EXPECT_EQ(date + resolveInterval, Date(2026, 3, 2));
}

TEST(DateTest, OperatorPlusAddsNonMonthIntervals) {
    auto date{Date(2026, 1, 3)}; // not a leap year

    auto interval{hbt::mods::Interval({.years = 1, .weeks = 1, .days = 1})};

    EXPECT_EQ(date + interval, Date(2027, 1, 11));
}

TEST(DateTest, OperatorPlusAddsMixedIntervals) {
    auto date{Date(2026, 1, 30)}; // not a leap year, 31 days in January

    auto resolveNoOverflow{hbt::mods::Interval(
        {.years = 1, .months = 1, .weeks = 1, .days = 1},
        hbt::mods::Interval::MonthHandling::RESOLVE_OVERFLOW)};
    EXPECT_EQ(date + resolveNoOverflow, Date(2027, 3, 7));

    auto resolveOverflow{
        hbt::mods::Interval({.years = 1, .months = 1, .days = 1})};
    EXPECT_EQ(date + resolveOverflow, Date(2027, 3, 3));

    auto clampNoClamp{
        hbt::mods::Interval({.years = 1, .months = 1, .weeks = 1, .days = 1},
                            hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampNoClamp, Date(2027, 3, 7));

    auto clampClamp{
        hbt::mods::Interval({.years = 1, .months = 1, .days = 1},
                            hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampClamp, Date(2027, 2, 28));
}

TEST(DateTest, OperatorMinusSubtractsMonthIntervals) {
    auto date{Date(2026, 3, 30)}; // not a leap year

    auto clampInterval{hbt::mods::Interval(
        {.months = 1}, hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date - clampInterval, Date(2026, 2, 28));

    auto resolveInterval{hbt::mods::Interval(
        {.months = 1}, hbt::mods::Interval::MonthHandling::RESOLVE_OVERFLOW)};
    EXPECT_EQ(date - resolveInterval, Date(2026, 3, 2));
}

TEST(DateTest, OperatorMinusSubtractsNonMonthIntervals) {
    auto date{Date(2026, 3, 3)}; // not a leap year

    auto interval{hbt::mods::Interval({.years = 1, .weeks = 1, .days = 1})};

    EXPECT_EQ(date - interval, Date(2025, 2, 23));
}

TEST(DateTest, OperatorMinusSubtractsMixedIntervals) {
    auto date{Date(2026, 3, 30)}; // not a leap year

    auto resolveNoOverflow{hbt::mods::Interval(
        {.years = 1, .months = 1, .weeks = 1, .days = 1},
        hbt::mods::Interval::MonthHandling::RESOLVE_OVERFLOW)};
    EXPECT_EQ(date - resolveNoOverflow, Date(2025, 2, 22));

    auto resolveOverflow{
        hbt::mods::Interval({.years = 1, .months = 1, .days = 1})};
    EXPECT_EQ(date - resolveOverflow, Date(2025, 3, 1));

    auto clampNoClamp{
        hbt::mods::Interval({.years = 1, .months = 1, .weeks = 1, .days = 1},
                            hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampNoClamp, Date(2025, 2, 22));

    auto clampClamp{
        hbt::mods::Interval({.years = 1, .months = 1, .days = 1},
                            hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampClamp, Date(2025, 2, 28));
}
} // namespace test::mods
