#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>

#include <chrono>

namespace test::mods {
using hbt::mods::DateTime;
using hbt::mods::Interval;

TEST(DateTimeTest, GetYMDComponents) {
    auto datetime{DateTime{}};
    auto date{datetime.getDate()};

    EXPECT_EQ(datetime.getYear(), date.year());
    EXPECT_EQ(datetime.getMonth(), date.month());
    EXPECT_EQ(datetime.getDay(), date.day());
}

TEST(DateTimeTest, DefaultConstructorCreatesToday) {
    using namespace std::chrono;

    auto datetime{DateTime{}};
    auto currentYMD{year_month_day(floor<days>(system_clock::now()))};

    EXPECT_EQ(datetime.getDate(), DateTime{currentYMD}.getDate());
}

TEST(DateTimeTest, YMDConstructor) {
    using namespace std::chrono;
    auto currentYMD{year_month_day(floor<days>(system_clock::now()))};

    auto today1{DateTime{}};
    auto today2{DateTime{currentYMD}};

    EXPECT_EQ(today1.getDate(), today2.getDate());
}

TEST(DateTimeTest, YMDComponentsConstructor) {
    auto datetime{DateTime{std::chrono::year{2026}, std::chrono::month{3},
                           std::chrono::day{24}}};

    EXPECT_EQ(datetime.getYear(), std::chrono::year{2026});
    EXPECT_EQ(datetime.getMonth(), std::chrono::month{3});
    EXPECT_EQ(datetime.getDay(), std::chrono::day{24});
}

TEST(DateTimeTest, TodayFactoryFunction) {
    auto today1{DateTime::today()};
    auto today2{DateTime{}};

    EXPECT_EQ(today1.getDate(), today2.getDate());
}

TEST(DateTimeTest, IsTodayTrueOnTodayDates) {
    using namespace std::chrono;

    auto today1{DateTime::today()};
    EXPECT_TRUE(today1.isToday());

    auto today2{DateTime{}};
    EXPECT_TRUE(today2.isToday());

    auto currentYMD{year_month_day(floor<days>(system_clock::now()))};
    auto today3{DateTime{currentYMD}};
    EXPECT_TRUE(today3.isToday());
}

TEST(DateTimeTest, IsTodayFalseOnWrongDates) {
    using namespace std::chrono;
    auto tommorowYMD{
        year_month_day(floor<days>(system_clock::now()) + days{1})};

    auto tommorow{DateTime{tommorowYMD}};
    EXPECT_FALSE(tommorow.isToday());
}

TEST(DateTimeTest, ToFromISO8601String) {
    auto original{DateTime{std::chrono::year{2026}, std::chrono::month{3},
                           std::chrono::day{24}}};
    auto ymdString{original.toISO8601String()};

    auto restored{DateTime::fromISO8601String(ymdString)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored, original);
}

TEST(DateTimeTest, FromValidISO8601String) {
    EXPECT_TRUE(DateTime::fromISO8601String("2024-12-01T12:30").has_value());
    EXPECT_TRUE(DateTime::fromISO8601String("2024/12/01T00:30:12").has_value());
    EXPECT_TRUE(DateTime::fromISO8601String("2024.12.01T23:50:00").has_value());
    EXPECT_TRUE(DateTime::fromISO8601String("2024-12-01T02:10").has_value());
}

TEST(DateTimeTest, FromInvalidISO8601String) {
    EXPECT_FALSE(DateTime::fromISO8601String("").has_value());
    EXPECT_FALSE(DateTime::fromISO8601String("hello").has_value());

    EXPECT_FALSE(DateTime::fromISO8601String("2026").has_value());

    EXPECT_FALSE(DateTime::fromISO8601String("20261231").has_value());

    EXPECT_FALSE(DateTime::fromISO8601String("2026-12").has_value());

    // missing time section
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01").has_value());
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T12").has_value());
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T12:").has_value());

    // invalid hour
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T24:00").has_value());
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T-1:00").has_value());

    // invalid minute
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T22:61").has_value());
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T22:-1").has_value());

    // invalid month
    EXPECT_FALSE(DateTime::fromISO8601String("2026-13-31").has_value());

    // invalid day
    EXPECT_FALSE(DateTime::fromISO8601String("2026-13-38").has_value());

    // non-leap year Febrary 29th
    EXPECT_FALSE(DateTime::fromISO8601String("2026-02-29").has_value());

    // single digit sections
    EXPECT_FALSE(DateTime::fromISO8601String("2026-2-29").has_value());
    EXPECT_FALSE(DateTime::fromISO8601String("2026-02-1").has_value());
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T12:1").has_value());
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T2:10").has_value());
}

TEST(DateTimeTest, AddNonMonthInterval) {
    using namespace std::chrono;

    auto datetime{DateTime{}};

    auto yearInterval{Interval{{.years = 1}}};
    auto ymdYearFromNow{year_month_day(floor<days>(system_clock::now())) +
                        years{1}};
    EXPECT_EQ(datetime + yearInterval, DateTime{ymdYearFromNow});

    auto twoYearInterval{Interval{{.years = 2}}};
    auto ymdTwoYearsFromNow{year_month_day(floor<days>(system_clock::now())) +
                            years{2}};
    EXPECT_EQ(datetime + twoYearInterval, DateTime{ymdTwoYearsFromNow});

    auto weekInterval{Interval{{.weeks = 1}}};
    auto ymdWeekFromNow{
        year_month_day(floor<days>(system_clock::now()) + weeks{1})};
    EXPECT_EQ(datetime + weekInterval, DateTime{ymdWeekFromNow});

    auto twoWeekInterval{Interval{{.weeks = 2}}};
    auto ymdTwoWeeksFromNow{
        year_month_day(floor<days>(system_clock::now()) + weeks{2})};
    EXPECT_EQ(datetime + twoWeekInterval, DateTime{ymdTwoWeeksFromNow});

    auto dayInterval{Interval{{.days = 1}}};
    auto ymdDayFromNow{
        year_month_day(floor<days>(system_clock::now()) + days{1})};
    EXPECT_EQ(datetime + dayInterval, DateTime{ymdDayFromNow});

    auto twoDayInterval{Interval{{.days = 2}}};
    auto ymdTwoDaysFromNow{
        year_month_day(floor<days>(system_clock::now()) + days{2})};
    EXPECT_EQ(datetime + twoDayInterval, DateTime{ymdTwoDaysFromNow});

    auto yearWeekDayInterval{Interval{{.years = 1, .weeks = 1, .days = 1}}};
    auto ymdYearWeekDayFromNow{
        year_month_day(floor<days>(system_clock::now()) + days{1} + weeks{1}) +
        years{1}};
    EXPECT_EQ(datetime + yearWeekDayInterval, DateTime{ymdYearWeekDayFromNow});

    auto doubleYearWeekDayInterval{
        Interval{{.years = 2, .weeks = 2, .days = 2}}};
    auto ymdDoubleYearWeekDayFromNow{
        year_month_day(floor<days>(system_clock::now()) + days{2} + weeks{2}) +
        years{2}};
    EXPECT_EQ(datetime + doubleYearWeekDayInterval,
              DateTime{ymdDoubleYearWeekDayFromNow});
}

TEST(DateTimeTest, AddMonthInterval) {
    using namespace std::chrono;

    auto cutOffInterval{
        Interval{{.months = 1}, Interval::MonthHandling::CUT_OFF}};
    auto doubleCutOffInterval{
        Interval{{.months = 2}, Interval::MonthHandling::CUT_OFF}};

    auto wrapAroundInterval{
        Interval{{.months = 1}, Interval::MonthHandling::WRAP_AROUND}};
    auto doubleWrapAroundInterval{
        Interval{{.months = 2}, Interval::MonthHandling::WRAP_AROUND}};

    // There is 28 days in Feb 2026
    auto date1{DateTime{year{2026}, month{1}, day{29}}};
    auto date1CutOff{DateTime{year{2026}, month{2}, day{28}}};
    auto date1WrapAround{DateTime{year{2026}, month{3}, day{1}}};
    EXPECT_EQ(date1 + cutOffInterval, date1CutOff);
    EXPECT_EQ(date1 + wrapAroundInterval, date1WrapAround);

    auto date2{DateTime{year{2025}, month{12}, day{29}}};
    auto date2CutOff{DateTime{year{2026}, month{2}, day{28}}};
    auto date2WrapAround{DateTime{year{2026}, month{3}, day{1}}};
    EXPECT_EQ(date2 + doubleCutOffInterval, date2CutOff);
    EXPECT_EQ(date2 + doubleWrapAroundInterval, date2WrapAround);
}
} // namespace test::mods
