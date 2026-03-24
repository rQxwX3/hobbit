#include <gtest/gtest.h>

#include <date.hpp>
#include <interval.hpp>

#include <chrono>

namespace test::mods {
using hbt::mods::Date;
using hbt::mods::Interval;

TEST(DateTest, GetYMDComponents) {
    auto date{Date{}};
    auto ymd{date.getYMD()};

    EXPECT_EQ(date.getYear(), ymd.year());
    EXPECT_EQ(date.getMonth(), ymd.month());
    EXPECT_EQ(date.getDay(), ymd.day());
}

TEST(DateTest, DefaultConstructorCreatesToday) {
    using namespace std::chrono;

    auto date{Date{}};
    auto currentYMD{year_month_day(floor<days>(system_clock::now()))};

    EXPECT_EQ(date, Date{currentYMD});
}

TEST(DateTest, YMDConstructor) {
    using namespace std::chrono;
    auto currentYMD{year_month_day(floor<days>(system_clock::now()))};

    auto today1{Date{}};
    auto today2{Date{currentYMD}};

    EXPECT_EQ(today1, today2);
}

TEST(DateTest, YMDComponentsConstructor) {
    auto date{Date{std::chrono::year{2026}, std::chrono::month{3},
                   std::chrono::day{24}}};

    EXPECT_EQ(date.getYear(), std::chrono::year{2026});
    EXPECT_EQ(date.getMonth(), std::chrono::month{3});
    EXPECT_EQ(date.getDay(), std::chrono::day{24});
}

TEST(DateTest, TodayFactoryFunction) {
    auto today1{Date::today()};
    auto today2{Date{}};

    EXPECT_EQ(today1, today2);
}

TEST(DateTest, IsTodayTrueOnTodayDates) {
    using namespace std::chrono;

    auto today1{Date::today()};
    EXPECT_TRUE(today1.isToday());

    auto today2{Date{}};
    EXPECT_TRUE(today2.isToday());

    auto currentYMD{year_month_day(floor<days>(system_clock::now()))};
    auto today3{Date{currentYMD}};
    EXPECT_TRUE(today3.isToday());
}

TEST(DateTest, IsTodayFalseOnWrongDates) {
    using namespace std::chrono;
    auto tommorowYMD{
        year_month_day(floor<days>(system_clock::now()) + days{1})};

    auto tommorow{Date{tommorowYMD}};
    EXPECT_FALSE(tommorow.isToday());
}

TEST(DateTest, ToFromISO8601String) {
    auto original{Date{}};
    auto ymdString{original.toISO8601String()};

    auto restored{Date::fromISO8601String(ymdString)};
    EXPECT_EQ(restored, original);
}

TEST(DateTest, AddNonMonthInterval) {
    using namespace std::chrono;

    auto date{Date{}};

    auto yearInterval{Interval{{.years = 1}}};
    auto ymdYearFromNow{year_month_day(floor<days>(system_clock::now())) +
                        years{1}};
    EXPECT_EQ(date + yearInterval, Date{ymdYearFromNow});

    auto weekInterval{Interval{{.weeks = 1}}};
    auto ymdWeekFromNow{
        year_month_day(floor<days>(system_clock::now()) + weeks{1})};
    EXPECT_EQ(date + weekInterval, Date{ymdWeekFromNow});

    auto dayInterval{Interval{{.days = 1}}};
    auto ymdDayFromNow{
        year_month_day(floor<days>(system_clock::now()) + days{1})};
    EXPECT_EQ(date + dayInterval, Date{ymdDayFromNow});

    auto yearWeekDayInterval{Interval{{.years = 1, .weeks = 1, .days = 1}}};
    auto ymdYearWeekDayFromNow{
        year_month_day(floor<days>(system_clock::now()) + days{1} + weeks{1}) +
        years{1}};
    EXPECT_EQ(date + yearWeekDayInterval, Date{ymdYearWeekDayFromNow});
}

TEST(DateTest, AddMonthInterval) {
    using namespace std::chrono;

    auto cutOffInterval{
        Interval{{.months = 1}, Interval::MonthHandling::CUT_OFF}};
    auto wrapAroundInterval{
        Interval{{.months = 1}, Interval::MonthHandling::WRAP_AROUND}};

    // There is 28 days in Feb 2026
    auto date{Date{year{2026}, month{1}, day{29}}};
    auto dateCutOff{Date{year{2026}, month{2}, day{28}}};
    auto dateWrapAround{Date{year{2026}, month{3}, day{1}}};

    EXPECT_EQ(date + cutOffInterval, dateCutOff);
    EXPECT_EQ(date + wrapAroundInterval, dateWrapAround);
}
} // namespace test::mods
