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

TEST(DateTest, ThrowsOnInvalidYMD) {
    EXPECT_THROW(Date(year_month_day{year{2026}, month{2}, day{29}}),
                 std::invalid_argument); // not a leap year

    EXPECT_THROW(Date(year_month_day{year{-2026}, month{2}, day{29}}),
                 std::invalid_argument); // not a leap year
}

TEST(DateTest, ThrowsOnInvalidYearMonthDay) {
    EXPECT_THROW(Date(year{2026}, month{13}, day{13}), std::invalid_argument);

    EXPECT_THROW(Date(year{2026}, month{12}, day{32}), std::invalid_argument);

    EXPECT_THROW(Date(year{2026}, month{2}, day{29}),
                 std::invalid_argument); // not a leap year

    EXPECT_THROW(Date(year{2026}, month{13}, day{32}), std::invalid_argument);
}

TEST(DateTest, Getters) {
    auto ymd{year_month_day{year{2026}, month{4}, day{27}}};
    auto d1{Date(ymd)};
    EXPECT_EQ(d1.getYMD(), ymd);
    EXPECT_EQ(d1.getYear(), 2026);
    EXPECT_EQ(d1.getMonth(), 4);
    EXPECT_EQ(d1.getDay(), 27);
    EXPECT_EQ(d1.getWeekday(), Date::weekday_t::MONDAY);

    auto d2{Date(year{2026}, month{4}, day{27})};
    EXPECT_EQ(d2.getYMD(), ymd);
    EXPECT_EQ(d2.getYear(), 2026);
    EXPECT_EQ(d2.getMonth(), 4);
    EXPECT_EQ(d2.getDay(), 27);
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

TEST(DateTest, OperatorPlus) {
    auto date{Date(year_month_day{year{2026}, month{4}, day{26}})};

    auto cutOffInterval{
        hbt::mods::Interval(hbt::mods::Duration::Units{.days{5}},
                            hbt::mods::Interval::MonthHandling::CUT_OFF)};
    EXPECT_EQ(date + cutOffInterval,
              Date{year_month_day(year{2026}, month{4}, day{30})});

    auto wrapAroundInterval{
        hbt::mods::Interval(hbt::mods::Duration::Units{.days{5}},
                            hbt::mods::Interval::MonthHandling::WRAP_AROUND)};
    EXPECT_EQ(date + wrapAroundInterval,
              Date{year_month_day(year{2026}, month{5}, day{1})});
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
