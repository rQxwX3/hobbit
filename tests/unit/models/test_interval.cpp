#include <gtest/gtest.h>

#include <interval.hpp>

namespace test::mods {
using hbt::mods::Interval;
using unit_t = hbt::mods::Interval::unit_t;
TEST(IntervalTest, EmptyOnConstruction) {
    auto interval{Interval{}};
    EXPECT_EQ(interval.getMonthHandling(), Interval::defaultMonthHandling);
    EXPECT_EQ(interval.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::MINUTE), 0);
}

TEST(IntervalTest, HandlesEachUnit) {
    auto interval{Interval{{.years = 1,
                            .months = 2,
                            .weeks = 3,
                            .days = 4,
                            .hours = 5,
                            .minutes = 6}}};
    EXPECT_EQ(interval.getUnitValue(unit_t::YEAR), 1);
    EXPECT_EQ(interval.getUnitValue(unit_t::MONTH), 2);
    EXPECT_EQ(interval.getUnitValue(unit_t::WEEK), 3);
    EXPECT_EQ(interval.getUnitValue(unit_t::DAY), 4);
    EXPECT_EQ(interval.getUnitValue(unit_t::HOUR), 5);
    EXPECT_EQ(interval.getUnitValue(unit_t::MINUTE), 6);
}

TEST(IntervalTest, FactoryFunctions) {
    auto year{Interval::years(1)};
    EXPECT_EQ(year.getUnitValue(unit_t::YEAR), 1);
    EXPECT_EQ(year.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(year.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(year.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(year.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(year.getUnitValue(unit_t::MINUTE), 0);

    auto month{Interval::months(1)};
    EXPECT_EQ(month.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(month.getUnitValue(unit_t::MONTH), 1);
    EXPECT_EQ(month.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(month.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(month.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(month.getUnitValue(unit_t::MINUTE), 0);

    auto week{Interval::weeks(1)};
    EXPECT_EQ(week.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(week.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(week.getUnitValue(unit_t::WEEK), 1);
    EXPECT_EQ(week.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(week.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(week.getUnitValue(unit_t::MINUTE), 0);

    auto day{Interval::days(1)};
    EXPECT_EQ(day.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(day.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(day.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(day.getUnitValue(unit_t::DAY), 1);
    EXPECT_EQ(day.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(day.getUnitValue(unit_t::MINUTE), 0);

    auto hour{Interval::hours(1)};
    EXPECT_EQ(hour.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(hour.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(hour.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(hour.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(hour.getUnitValue(unit_t::HOUR), 1);
    EXPECT_EQ(hour.getUnitValue(unit_t::MINUTE), 0);

    auto minute{Interval::minutes(1)};
    EXPECT_EQ(minute.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(minute.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(minute.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(minute.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(minute.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(minute.getUnitValue(unit_t::MINUTE), 1);
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
