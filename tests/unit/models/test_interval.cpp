#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <interval.hpp>

namespace test::mods {
using hbt::mods::Interval;
using unit_t = Interval::unit_t;

[[nodiscard]] auto Interval123456() -> Interval {
    return Interval(Interval::struct_t{
        .years = 1,
        .months = 2,
        .weeks = 3,
        .days = 4,
        .hours = 5,
        .minutes = 6,
    });
}

TEST(IntervalTest, EmptyOnConstruction) {
    auto interval{Interval{}};

    EXPECT_EQ(interval.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(interval.getUnitValue(unit_t::MINUTE), 0);
}

TEST(IntervalTest, ArrayConstructor) {
    auto interval{Interval123456()};

    EXPECT_EQ(interval.getUnitValue(unit_t::YEAR), 1);
    EXPECT_EQ(interval.getUnitValue(unit_t::MONTH), 2);
    EXPECT_EQ(interval.getUnitValue(unit_t::WEEK), 3);
    EXPECT_EQ(interval.getUnitValue(unit_t::DAY), 4);
    EXPECT_EQ(interval.getUnitValue(unit_t::HOUR), 5);
    EXPECT_EQ(interval.getUnitValue(unit_t::MINUTE), 6);
}

TEST(IntervalTest, ArrayConstructorThrowsOnInvalidValue) {
    EXPECT_THROW(Interval(Interval::array_t{
                     Interval::maxValue + 1,
                     Interval::maxValue + 1,
                     Interval::maxValue + 1,
                     Interval::maxValue + 1,
                     Interval::maxValue + 1,
                     Interval::maxValue + 1,
                 }),
                 std::invalid_argument);
}

TEST(IntervalTest, StructConstructor) {
    Interval::Units u{.years = 1,
                      .months = 2,
                      .weeks = 3,
                      .days = 4,
                      .hours = 5,
                      .minutes = 6};
    auto interval{Interval{u}};

    EXPECT_EQ(interval.getUnitValue(unit_t::YEAR), 1);
    EXPECT_EQ(interval.getUnitValue(unit_t::MONTH), 2);
    EXPECT_EQ(interval.getUnitValue(unit_t::WEEK), 3);
    EXPECT_EQ(interval.getUnitValue(unit_t::DAY), 4);
    EXPECT_EQ(interval.getUnitValue(unit_t::HOUR), 5);
    EXPECT_EQ(interval.getUnitValue(unit_t::MINUTE), 6);
}

TEST(IntervalTest, StructConstructorThrowsOnInvalidValue) {
    EXPECT_THROW(Interval(Interval::struct_t{
                     .years = Interval::maxValue + 1,
                     .months = Interval::maxValue + 1,
                     .weeks = Interval::maxValue + 1,
                     .days = Interval::maxValue + 1,
                     .hours = Interval::maxValue + 1,
                     .minutes = Interval::maxValue + 1,
                 }),
                 std::invalid_argument);
}

TEST(IntervalTest, ConvertUnitsUpwards) {
    auto interval{Interval(Interval::array_t{1, 12, 3, 14, 24, 60})};

    auto res{interval.convertUnitsUpwards()};

    EXPECT_EQ(res.getUnitValue(unit_t::YEAR), 2);
    EXPECT_EQ(res.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::WEEK), 5);
    EXPECT_EQ(res.getUnitValue(unit_t::DAY), 1);
    EXPECT_EQ(res.getUnitValue(unit_t::HOUR), 1);
    EXPECT_EQ(res.getUnitValue(unit_t::MINUTE), 0);
}

TEST(IntervalTest, ConvertUnitsDownwards) {
    auto interval{Interval123456()};

    auto res{interval.convertUnitsDownwards()};

    EXPECT_EQ(res.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::MONTH), 14);
    EXPECT_EQ(res.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::MINUTE), 36306);
}

TEST(IntervalTest, FromUnit) {
    auto interval{Interval::fromUnit(unit_t::YEAR, 12)};
    EXPECT_EQ(interval.getUnitValue(unit_t::YEAR), 12);

    EXPECT_THROW(Interval::fromUnit(unit_t::MONTH, Interval::maxValue + 1),
                 std::invalid_argument);
}

TEST(IntervalTest, FactoryFunctions) {
    auto allExceptUnitAreNullAndUnitIsValue{
        [](Interval interval, unit_t exceptUnit,
           Interval::value_t value) -> bool {
            for (auto unit : Interval::units) {
                if (unit == exceptUnit &&
                    interval.getUnitValue(unit) != value) {
                    return false;
                }

                if (unit != exceptUnit && interval.getUnitValue(unit) != 0) {
                    return false;
                }
            }

            return true;
        }};

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Interval::years(12),
                                                   unit_t::YEAR, 12));

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Interval::months(11),
                                                   unit_t::MONTH, 11));

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Interval::weeks(10),
                                                   unit_t::WEEK, 10));
    EXPECT_TRUE(
        allExceptUnitAreNullAndUnitIsValue(Interval::days(9), unit_t::DAY, 9));

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Interval::hours(8),
                                                   unit_t::HOUR, 8));

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Interval::minutes(7),
                                                   unit_t::MINUTE, 7));
}

TEST(IntervalTest, GetNonZeroUnitValuePairs) {
    EXPECT_THAT(Interval123456().getNonZeroUnitValuePairs(),
                testing::ElementsAre(std::make_pair(unit_t::YEAR, 1),
                                     std::make_pair(unit_t::MONTH, 2),
                                     std::make_pair(unit_t::WEEK, 3),
                                     std::make_pair(unit_t::DAY, 4),
                                     std::make_pair(unit_t::HOUR, 5),
                                     std::make_pair(unit_t::MINUTE, 6)));

    EXPECT_THAT(Interval(Interval::array_t{1, 0, 1, 0, 1, 0})
                    .getNonZeroUnitValuePairs(),
                testing::ElementsAre(std::make_pair(unit_t::YEAR, 1),
                                     std::make_pair(unit_t::WEEK, 1),
                                     std::make_pair(unit_t::HOUR, 1)));

    EXPECT_THAT(Interval(Interval::array_t{0, 0, 0, 0, 0, 1})
                    .getNonZeroUnitValuePairs(),
                testing::ElementsAre(std::make_pair(unit_t::MINUTE, 1)));

    EXPECT_TRUE(Interval().getNonZeroUnitValuePairs().empty());
}

TEST(IntervalTest, IsZero) {
    auto interval{Interval{}};
    EXPECT_TRUE(interval.isZero());

    interval = Interval{Interval::array_t{1, 0, 0, 0, 0, 0}};
    EXPECT_FALSE(interval.isZero());
}

TEST(IntervalTest, AddGetUnits) {
    auto interval{Interval{}};

    interval.addUnit(unit_t::YEAR, 1);
    EXPECT_EQ(interval.getUnitValue(unit_t::YEAR), 1);

    interval.addUnit(unit_t::MONTH, 1);
    EXPECT_EQ(interval.getUnitValue(unit_t::MONTH), 1);

    interval.addUnit(unit_t::WEEK, 1);
    EXPECT_EQ(interval.getUnitValue(unit_t::WEEK), 1);

    interval.addUnit(unit_t::DAY, 1);
    EXPECT_EQ(interval.getUnitValue(unit_t::DAY), 1);

    interval.addUnit(unit_t::HOUR, 1);
    EXPECT_EQ(interval.getUnitValue(unit_t::HOUR), 1);

    interval.addUnit(unit_t::MINUTE, 1);
    EXPECT_EQ(interval.getUnitValue(unit_t::MINUTE), 1);
}

TEST(IntervalTest, OperatorPlus) {
    auto first{Interval{}};
    first.addUnit(unit_t::YEAR, 1);
    first.addUnit(unit_t::MONTH, 1);
    first.addUnit(unit_t::WEEK, 1);
    first.addUnit(unit_t::DAY, 1);
    first.addUnit(unit_t::HOUR, 1);
    first.addUnit(unit_t::MINUTE, 1);

    auto second{Interval{}};
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

TEST(IntervalTest, ComparisonOperators) {
    // TODO compare 1y with 0y100months
    auto first{Interval{}};
    auto second{Interval{}};
    EXPECT_FALSE(first > second);
    EXPECT_FALSE(first < second);
    EXPECT_TRUE(first == second);
    EXPECT_FALSE(first != second);

    first = Interval::days(1);
    EXPECT_TRUE(first > second);
    EXPECT_TRUE(second < first);
    EXPECT_TRUE(second != first);
    EXPECT_FALSE(second == first);

    first = Interval::years(1);
    second = Interval::months(100);
    EXPECT_TRUE(second > first);
}

TEST(IntervalTest, OnlyContainsUnit) {
    EXPECT_TRUE(Interval::days(5).onlyContainsUnit(unit_t::DAY));

    EXPECT_FALSE(Interval123456().onlyContainsUnit(unit_t::MONTH));

    EXPECT_FALSE(Interval().onlyContainsUnit(unit_t::MONTH));
}

TEST(IntervalTest, IsMultipleOf) {
    auto i1{Interval::days(10)};
    auto i2{Interval::days(5)};
    EXPECT_TRUE(i1.isMultipleOf(i2));

    i1 = Interval::weeks(2);
    i1 = Interval::weeks(3);
    EXPECT_FALSE(i1.isMultipleOf(i2));

    i1 = Interval{Interval::Units{.years = 2,
                                  .months = 2,
                                  .weeks = 2,
                                  .days = 2,
                                  .hours = 2,
                                  .minutes = 2}};

    i2 = Interval{Interval::Units{.years = 1,
                                  .months = 1,
                                  .weeks = 1,
                                  .days = 1,
                                  .hours = 1,
                                  .minutes = 1}};

    EXPECT_TRUE(i1.isMultipleOf(i2));
}
} // namespace test::mods
