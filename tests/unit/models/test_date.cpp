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

TEST(DateTest, ThrowsOnInvalidChronoYMD) {
    EXPECT_THROW(Date(year_month_day{year{2026}, month{2}, day{29}}),
                 std::invalid_argument); // not a leap year

    EXPECT_THROW(Date(year_month_day{year{2000}, month{13}, day{10}}),
                 std::invalid_argument);

    EXPECT_THROW(Date(year_month_day{year{2000}, month{12}, day{32}}),
                 std::invalid_argument);

    EXPECT_THROW(Date(year_month_day{year{2000}, month{0}, day{28}}),
                 std::invalid_argument);

    EXPECT_THROW(Date(year_month_day{year{2000}, month{12}, day{0}}),
                 std::invalid_argument);

    EXPECT_THROW(Date(year_month_day{year{2000}, month{0}, day{0}}),
                 std::invalid_argument);

    EXPECT_THROW(Date(year_month_day{year{2000}, month{13}, day{32}}),
                 std::invalid_argument);
}

TEST(DateTest, ThrowsOnInvalidYMD) {
    EXPECT_THROW(Date({.year = 2026, .month = 2, .day = 29}),
                 std::invalid_argument); // not a leap year
                                         //
    EXPECT_THROW(Date({.year = 2000, .month = 13, .day = 10}),
                 std::invalid_argument);

    EXPECT_THROW(Date({.year = 2000, .month = 12, .day = 32}),
                 std::invalid_argument);

    EXPECT_THROW(Date({.year = 2000, .month = 0, .day = 28}),
                 std::invalid_argument);

    EXPECT_THROW(Date({.year = 2000, .month = 12, .day = 0}),
                 std::invalid_argument);

    EXPECT_THROW(Date({.year = 2000, .month = 0, .day = 0}),
                 std::invalid_argument);

    EXPECT_THROW(Date({.year = 2000, .month = 12, .day = 32}),
                 std::invalid_argument);
}

TEST(DateTest, ThrowsOnInvalidYearMonthDay) {
    EXPECT_THROW(Date(year{2026}, month{2}, day{29}),
                 std::invalid_argument); // not a leap year
                                         //
    EXPECT_THROW(Date(year{2000}, month{13}, day{10}), std::invalid_argument);

    EXPECT_THROW(Date(year{2000}, month{12}, day{32}), std::invalid_argument);

    EXPECT_THROW(Date(year{2000}, month{0}, day{28}), std::invalid_argument);

    EXPECT_THROW(Date(year{2000}, month{12}, day{0}), std::invalid_argument);

    EXPECT_THROW(Date(year{2000}, month{0}, day{0}), std::invalid_argument);

    EXPECT_THROW(Date(year{2000}, month{12}, day{32}), std::invalid_argument);
}

TEST(DateTest, Getters) {
    auto chronoYMD{year_month_day{year{2026}, month{4}, day{27}}};
    auto d1{Date(chronoYMD)};
    EXPECT_EQ(d1.getChronoYMD(), chronoYMD);
    EXPECT_EQ(d1.getYear(), year(2026));
    EXPECT_EQ(d1.getMonth(), month(4));
    EXPECT_EQ(d1.getDay(), day(27));
    EXPECT_EQ(d1.getWeekday(), Date::weekday_t::MONDAY);

    auto d2{Date(year{2026}, month{4}, day{27})};
    EXPECT_EQ(d2.getChronoYMD(), chronoYMD);
    EXPECT_EQ(d2.getYear(), year(2026));
    EXPECT_EQ(d2.getMonth(), month(4));
    EXPECT_EQ(d2.getDay(), day(27));
    EXPECT_EQ(d2.getWeekday(), Date::weekday_t::MONDAY);
}

TEST(DateTest, IsTodayTrueOnCorrectDates) {
    auto today{Date::today()};
    EXPECT_TRUE(today.isToday());

    auto d2{Date(year{2026}, month{4}, day{26})};
    EXPECT_FALSE(d2.isToday());
}

TEST(DateTest, IsTodayFalseOnWrongDates) {
    auto date{Date{year_month_day(year{2000}, month{12}, day{31})}};
    EXPECT_FALSE(date.isToday());
}

TEST(DateTest, ComparisonOperators) {
    auto d1{Date(year_month_day{year{2026}, month{4}, day{26}})};
    auto d2{Date(year_month_day{year{2026}, month{4}, day{27}})};

    EXPECT_TRUE(d1 < d2);
    EXPECT_TRUE(d1 <= d2);

    EXPECT_TRUE(d2 > d1);
    EXPECT_TRUE(d2 >= d1);

    EXPECT_FALSE(d1 == d2);
    EXPECT_TRUE(d1 != d2);

    auto d3{Date(year{2026}, month{4}, day{26})};
    auto d4{Date(year{2026}, month{4}, day{27})};

    EXPECT_TRUE(d3 < d4);
    EXPECT_TRUE(d3 <= d4);

    EXPECT_TRUE(d4 > d3);
    EXPECT_TRUE(d4 >= d3);

    EXPECT_FALSE(d3 == d4);
    EXPECT_TRUE(d3 != d4);
}

TEST(DateTest, OperatorPlusAddsMonthIntervals) {
    auto date{Date({2026, 1, 30})}; // not a leap year

    auto clampInterval{hbt::mods::Interval(
        {.months = 1}, hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampInterval, Date({2026, 2, 28}));

    auto resolveInterval{hbt::mods::Interval(
        {.months = 1}, hbt::mods::Interval::MonthHandling::RESOLVE_OVERFLOW)};
    EXPECT_EQ(date + resolveInterval, Date({2026, 3, 2}));
}

TEST(DateTest, OperatorPlusAddsNonMonthIntervals) {
    auto date{Date({2026, 1, 3})}; // not a leap year

    auto interval{hbt::mods::Interval({.years = 1, .weeks = 1, .days = 1})};

    EXPECT_EQ(date + interval, Date({2027, 1, 11}));
}

TEST(DateTest, OperatorPlusAddsMixedIntervals) {
    auto date{Date({2026, 1, 30})}; // not a leap year, 31 days in January

    auto resolveNoOverflow{hbt::mods::Interval(
        {.years = 1, .months = 1, .weeks = 1, .days = 1},
        hbt::mods::Interval::MonthHandling::RESOLVE_OVERFLOW)};
    EXPECT_EQ(date + resolveNoOverflow, Date({2027, 3, 7}));

    auto resolveOverflow{
        hbt::mods::Interval({.years = 1, .months = 1, .days = 1})};
    EXPECT_EQ(date + resolveOverflow, Date({2027, 3, 3}));

    auto clampNoClamp{
        hbt::mods::Interval({.years = 1, .months = 1, .weeks = 1, .days = 1},
                            hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampNoClamp, Date({2027, 3, 7}));

    auto clampClamp{
        hbt::mods::Interval({.years = 1, .months = 1, .days = 1},
                            hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampClamp, Date({2027, 2, 28}));
}

TEST(DateTest, OperatorMinusSubtractsMonthIntervals) {
    auto date{Date({2026, 3, 30})}; // not a leap year

    auto clampInterval{hbt::mods::Interval(
        {.months = 1}, hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date - clampInterval, Date({2026, 2, 28}));

    auto resolveInterval{hbt::mods::Interval(
        {.months = 1}, hbt::mods::Interval::MonthHandling::RESOLVE_OVERFLOW)};
    EXPECT_EQ(date - resolveInterval, Date({2026, 2, 26}));
}

TEST(DateTest, OperatorMinusSubtractsNonMonthIntervals) {
    auto date{Date({2026, 3, 3})}; // not a leap year

    auto interval{hbt::mods::Interval({.years = 1, .weeks = 1, .days = 1})};

    EXPECT_EQ(date - interval, Date({2025, 2, 23}));
}

TEST(DateTest, OperatorMinusSubtractsMixedIntervals) {
    auto date{Date({2026, 3, 30})}; // not a leap year, 31 days in January

    auto resolveNoOverflow{hbt::mods::Interval(
        {.years = 1, .months = 1, .weeks = 1, .days = 1},
        hbt::mods::Interval::MonthHandling::RESOLVE_OVERFLOW)};
    EXPECT_EQ(date - resolveNoOverflow, Date({2025, 2, 22}));

    auto resolveOverflow{
        hbt::mods::Interval({.years = 1, .months = 1, .days = 1})};
    EXPECT_EQ(date - resolveOverflow, Date({2025, 2, 27}));

    auto clampNoClamp{
        hbt::mods::Interval({.years = 1, .months = 1, .weeks = 1, .days = 1},
                            hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampNoClamp, Date({2025, 2, 22}));

    auto clampClamp{
        hbt::mods::Interval({.years = 1, .months = 1, .days = 1},
                            hbt::mods::Interval::MonthHandling::CLAMP_TO_END)};
    EXPECT_EQ(date + clampClamp, Date({2025, 2, 27}));
}

TEST(DateTest, GetDiff) {
    auto d1{Date(year_month_day{year{2026}, month{4}, day{26}})};
    auto d2{Date(year_month_day{year{2025}, month{3}, day{25}})};

    EXPECT_EQ(Date::getDiff(d1, d2),
              hbt::mods::Duration(
                  hbt::mods::Duration::Units{.years{1}, .months{1}, .days{1}}));

    EXPECT_EQ(Date::getDiff(d2, d1),
              hbt::mods::Duration(
                  hbt::mods::Duration::Units{.years{1}, .months{1}, .days{1}}));
}
} // namespace test::mods
