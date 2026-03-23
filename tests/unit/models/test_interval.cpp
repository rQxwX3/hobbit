#include <gtest/gtest.h>

#include <interval.hpp>

namespace test::mods {
using hbt::mods::Interval;
TEST(IntervalTest, EmptyOnConstruction) {
    auto interval{Interval{}};
    EXPECT_EQ(interval.getMonthHandling(), Interval::defaultMonthHandling);
    EXPECT_EQ(interval.getYears(), 0);
    EXPECT_EQ(interval.getMonths(), 0);
    EXPECT_EQ(interval.getWeeks(), 0);
    EXPECT_EQ(interval.getDays(), 0);
    EXPECT_EQ(interval.getHours(), 0);
    EXPECT_EQ(interval.getMinutes(), 0);
}

TEST(IntervalTest, FactoryFunctions) {
    auto year{Interval::years(1)};
    EXPECT_EQ(year.getYears(), 1);
    EXPECT_EQ(year.getMonths(), 0);
    EXPECT_EQ(year.getWeeks(), 0);
    EXPECT_EQ(year.getDays(), 0);
    EXPECT_EQ(year.getHours(), 0);
    EXPECT_EQ(year.getMinutes(), 0);

    auto month{Interval::months(1)};
    EXPECT_EQ(month.getYears(), 0);
    EXPECT_EQ(month.getMonths(), 1);
    EXPECT_EQ(month.getWeeks(), 0);
    EXPECT_EQ(month.getDays(), 0);
    EXPECT_EQ(month.getHours(), 0);
    EXPECT_EQ(month.getMinutes(), 0);

    auto week{Interval::weeks(1)};
    EXPECT_EQ(week.getYears(), 0);
    EXPECT_EQ(week.getMonths(), 0);
    EXPECT_EQ(week.getWeeks(), 1);
    EXPECT_EQ(week.getDays(), 0);
    EXPECT_EQ(week.getHours(), 0);
    EXPECT_EQ(week.getMinutes(), 0);

    auto day{Interval::days(1)};
    EXPECT_EQ(day.getYears(), 0);
    EXPECT_EQ(day.getMonths(), 0);
    EXPECT_EQ(day.getWeeks(), 0);
    EXPECT_EQ(day.getDays(), 1);
    EXPECT_EQ(day.getHours(), 0);
    EXPECT_EQ(day.getMinutes(), 0);

    auto hour{Interval::hours(1)};
    EXPECT_EQ(hour.getYears(), 0);
    EXPECT_EQ(hour.getMonths(), 0);
    EXPECT_EQ(hour.getWeeks(), 0);
    EXPECT_EQ(hour.getDays(), 0);
    EXPECT_EQ(hour.getHours(), 1);
    EXPECT_EQ(hour.getMinutes(), 0);

    auto minute{Interval::minutes(1)};
    EXPECT_EQ(minute.getYears(), 0);
    EXPECT_EQ(minute.getMonths(), 0);
    EXPECT_EQ(minute.getWeeks(), 0);
    EXPECT_EQ(minute.getDays(), 0);
    EXPECT_EQ(minute.getHours(), 0);
    EXPECT_EQ(minute.getMinutes(), 1);
}

TEST(IntervalTest, IsZero) {
    auto interval{Interval{}};
    EXPECT_TRUE(interval.isZero());

    auto year{Interval::years(1)};
    EXPECT_FALSE(year.isZero());
}

TEST(IntervalTest, ToFromJSON) {
    auto original{Interval::years(1) + Interval::months(1) +
                  Interval::weeks(1) + Interval::days(1) + Interval::hours(1) +
                  Interval::minutes(1)};

    auto intervalJSON = original.toJSON();
    auto restored{Interval::fromJSON(intervalJSON)};

    EXPECT_EQ(original, restored);
}
}; // namespace test::mods
