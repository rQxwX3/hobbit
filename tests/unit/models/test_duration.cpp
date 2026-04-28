#include <duration.hpp>
#include <gtest/gtest.h>

namespace test::mods {

using hbt::mods::Duration;
using unit_t = Duration::unit_t;

TEST(DurationTest, EmptyOnConstruction) {
    auto duration{Duration{}};

    EXPECT_EQ(duration.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(duration.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(duration.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(duration.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(duration.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(duration.getUnitValue(unit_t::MINUTE), 0);
}

TEST(DurationTest, ArrayConstructor) {
    auto duration{Duration{Duration::array_t{1, 2, 3, 4, 5, 6}}};

    EXPECT_EQ(duration.getUnitValue(unit_t::YEAR), 1);
    EXPECT_EQ(duration.getUnitValue(unit_t::MONTH), 2);
    EXPECT_EQ(duration.getUnitValue(unit_t::WEEK), 3);
    EXPECT_EQ(duration.getUnitValue(unit_t::DAY), 4);
    EXPECT_EQ(duration.getUnitValue(unit_t::HOUR), 5);
    EXPECT_EQ(duration.getUnitValue(unit_t::MINUTE), 6);
}

TEST(DurationTest, StructConstructor) {
    Duration::Units u{.years = 1,
                      .months = 2,
                      .weeks = 3,
                      .days = 4,
                      .hours = 5,
                      .minutes = 6};
    auto duration{Duration{u}};

    EXPECT_EQ(duration.getUnitValue(unit_t::YEAR), 1);
    EXPECT_EQ(duration.getUnitValue(unit_t::MONTH), 2);
    EXPECT_EQ(duration.getUnitValue(unit_t::WEEK), 3);
    EXPECT_EQ(duration.getUnitValue(unit_t::DAY), 4);
    EXPECT_EQ(duration.getUnitValue(unit_t::HOUR), 5);
    EXPECT_EQ(duration.getUnitValue(unit_t::MINUTE), 6);
}

TEST(DurationTest, IsZero) {
    auto duration{Duration{}};
    EXPECT_TRUE(duration.isZero());

    duration.addUnit(unit_t::DAY, 1);
    EXPECT_FALSE(duration.isZero());
}

TEST(DurationTest, AddGetUnits) {
    auto duration{Duration{}};

    duration.addUnit(unit_t::YEAR, 1);
    EXPECT_EQ(duration.getUnitValue(unit_t::YEAR), 1);

    duration.addUnit(unit_t::MONTH, 1);
    EXPECT_EQ(duration.getUnitValue(unit_t::MONTH), 1);

    duration.addUnit(unit_t::WEEK, 1);
    EXPECT_EQ(duration.getUnitValue(unit_t::WEEK), 1);

    duration.addUnit(unit_t::DAY, 1);
    EXPECT_EQ(duration.getUnitValue(unit_t::DAY), 1);

    duration.addUnit(unit_t::HOUR, 1);
    EXPECT_EQ(duration.getUnitValue(unit_t::HOUR), 1);

    duration.addUnit(unit_t::MINUTE, 1);
    EXPECT_EQ(duration.getUnitValue(unit_t::MINUTE), 1);
}

TEST(DurationTest, OperatorPlus) {
    auto first{Duration{}};
    first.addUnit(unit_t::YEAR, 1);
    first.addUnit(unit_t::MONTH, 1);
    first.addUnit(unit_t::WEEK, 1);
    first.addUnit(unit_t::DAY, 1);
    first.addUnit(unit_t::HOUR, 1);
    first.addUnit(unit_t::MINUTE, 1);

    auto second{Duration{}};
    second.addUnit(unit_t::YEAR, 1);
    second.addUnit(unit_t::MONTH, 2);
    second.addUnit(unit_t::WEEK, 3);
    second.addUnit(unit_t::DAY, 4);
    second.addUnit(unit_t::HOUR, 5);
    second.addUnit(unit_t::MINUTE, 6);

    auto result{first + second};

    EXPECT_EQ(result.getUnitValue(unit_t::YEAR), 2);
    EXPECT_EQ(result.getUnitValue(unit_t::MONTH), 3);
    EXPECT_EQ(result.getUnitValue(unit_t::WEEK), 4);
    EXPECT_EQ(result.getUnitValue(unit_t::DAY), 5);
    EXPECT_EQ(result.getUnitValue(unit_t::HOUR), 6);
    EXPECT_EQ(result.getUnitValue(unit_t::MINUTE), 7);
}

TEST(DurationTest, ComparisonOperators) {
    auto first{Duration{}};
    auto second{Duration{}};

    EXPECT_TRUE(first == second);

    first.addUnit(unit_t::DAY, 1);
    EXPECT_TRUE(first > second);

    second.addUnit(unit_t::DAY, 2);
    EXPECT_TRUE(second > first);

    first.addUnit(unit_t::MONTH, 1);
    EXPECT_TRUE(first > second);

    second.addUnit(unit_t::MONTH, 1);
    first.addUnit(unit_t::DAY, 1);

    EXPECT_TRUE(first == second);
}

TEST(DurationTest, OnlyContainsUnit) {
    auto d{Duration::days(5)};
    EXPECT_TRUE(d.onlyContainsUnit(unit_t::DAY));
    EXPECT_FALSE(d.onlyContainsUnit(unit_t::MONTH));
}

TEST(DurationTest, IsMultipleOf) {
    auto d1{Duration::days(10)};
    auto d2{Duration::days(5)};
    EXPECT_TRUE(d1.isMultipleOf(d2));

    d1 = Duration::weeks(2);
    d1 = Duration::weeks(3);
    EXPECT_FALSE(d1.isMultipleOf(d2));

    d1 = Duration{Duration::Units{.years = 2,
                                  .months = 2,
                                  .weeks = 2,
                                  .days = 2,
                                  .hours = 2,
                                  .minutes = 2}};

    d2 = Duration{Duration::Units{.years = 1,
                                  .months = 1,
                                  .weeks = 1,
                                  .days = 1,
                                  .hours = 1,
                                  .minutes = 1}};

    EXPECT_TRUE(d1.isMultipleOf(d2));
}
} // namespace test::mods
