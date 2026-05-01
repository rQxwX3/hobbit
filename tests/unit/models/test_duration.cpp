#include <duration.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test::mods {

using hbt::mods::Duration;
using unit_t = Duration::unit_t;

[[nodiscard]] auto Duration123456() -> Duration {
    return Duration(Duration::struct_t{
        .years = 1,
        .months = 2,
        .weeks = 3,
        .days = 4,
        .hours = 5,
        .minutes = 6,
    });
}

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
    auto duration{Duration123456()};

    EXPECT_EQ(duration.getUnitValue(unit_t::YEAR), 1);
    EXPECT_EQ(duration.getUnitValue(unit_t::MONTH), 2);
    EXPECT_EQ(duration.getUnitValue(unit_t::WEEK), 3);
    EXPECT_EQ(duration.getUnitValue(unit_t::DAY), 4);
    EXPECT_EQ(duration.getUnitValue(unit_t::HOUR), 5);
    EXPECT_EQ(duration.getUnitValue(unit_t::MINUTE), 6);
}

TEST(DurationTest, ArrayConstructorThrowsOnInvalidValue) {
    EXPECT_THROW(Duration(Duration::array_t{
                     Duration::maxValue + 1,
                     Duration::maxValue + 1,
                     Duration::maxValue + 1,
                     Duration::maxValue + 1,
                     Duration::maxValue + 1,
                     Duration::maxValue + 1,
                 }),
                 std::invalid_argument);
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

TEST(DurationTest, StructConstructorThrowsOnInvalidValue) {
    EXPECT_THROW(Duration(Duration::struct_t{
                     .years = Duration::maxValue + 1,
                     .months = Duration::maxValue + 1,
                     .weeks = Duration::maxValue + 1,
                     .days = Duration::maxValue + 1,
                     .hours = Duration::maxValue + 1,
                     .minutes = Duration::maxValue + 1,
                 }),
                 std::invalid_argument);
}

TEST(DurationTest, ConvertUnitsUpwards) {
    auto duration{Duration(Duration::array_t{1, 12, 3, 14, 24, 60})};

    auto res{duration.convertUnitsUpwards()};

    EXPECT_EQ(res.getUnitValue(unit_t::YEAR), 2);
    EXPECT_EQ(res.getUnitValue(unit_t::MONTH), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::WEEK), 5);
    EXPECT_EQ(res.getUnitValue(unit_t::DAY), 1);
    EXPECT_EQ(res.getUnitValue(unit_t::HOUR), 1);
    EXPECT_EQ(res.getUnitValue(unit_t::MINUTE), 0);
}

TEST(DurationTest, ConvertUnitsDownwards) {
    auto duration{Duration123456()};

    auto res{duration.convertUnitsDownwards()};

    EXPECT_EQ(res.getUnitValue(unit_t::YEAR), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::MONTH), 14);
    EXPECT_EQ(res.getUnitValue(unit_t::WEEK), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::DAY), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::HOUR), 0);
    EXPECT_EQ(res.getUnitValue(unit_t::MINUTE), 36306);
}

TEST(DurationTest, FromUnit) {
    auto duration{Duration::fromUnit(unit_t::YEAR, 12)};
    EXPECT_EQ(duration.getUnitValue(unit_t::YEAR), 12);

    EXPECT_THROW(Duration::fromUnit(unit_t::MONTH, Duration::maxValue + 1),
                 std::invalid_argument);
}

TEST(DurationTest, FactoryFunctions) {
    auto allExceptUnitAreNullAndUnitIsValue{
        [](Duration duration, unit_t exceptUnit,
           Duration::value_t value) -> bool {
            for (auto unit : Duration::units) {
                if (unit == exceptUnit &&
                    duration.getUnitValue(unit) != value) {
                    return false;
                }

                if (unit != exceptUnit && duration.getUnitValue(unit) != 0) {
                    return false;
                }
            }

            return true;
        }};

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Duration::years(12),
                                                   unit_t::YEAR, 12));

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Duration::months(11),
                                                   unit_t::MONTH, 11));

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Duration::weeks(10),
                                                   unit_t::WEEK, 10));
    EXPECT_TRUE(
        allExceptUnitAreNullAndUnitIsValue(Duration::days(9), unit_t::DAY, 9));

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Duration::hours(8),
                                                   unit_t::HOUR, 8));

    EXPECT_TRUE(allExceptUnitAreNullAndUnitIsValue(Duration::minutes(7),
                                                   unit_t::MINUTE, 7));
}

TEST(DurationTest, GetNonZeroUnitValuePairs) {
    EXPECT_THAT(Duration123456().getNonZeroUnitValuePairs(),
                testing::ElementsAre(std::make_pair(unit_t::YEAR, 1),
                                     std::make_pair(unit_t::MONTH, 2),
                                     std::make_pair(unit_t::WEEK, 3),
                                     std::make_pair(unit_t::DAY, 4),
                                     std::make_pair(unit_t::HOUR, 5),
                                     std::make_pair(unit_t::MINUTE, 6)));

    EXPECT_THAT(Duration(Duration::array_t{1, 0, 1, 0, 1, 0})
                    .getNonZeroUnitValuePairs(),
                testing::ElementsAre(std::make_pair(unit_t::YEAR, 1),
                                     std::make_pair(unit_t::WEEK, 1),
                                     std::make_pair(unit_t::HOUR, 1)));

    EXPECT_THAT(Duration(Duration::array_t{0, 0, 0, 0, 0, 1})
                    .getNonZeroUnitValuePairs(),
                testing::ElementsAre(std::make_pair(unit_t::MINUTE, 1)));

    EXPECT_TRUE(Duration().getNonZeroUnitValuePairs().empty());
}

TEST(DurationTest, IsZero) {
    auto duration{Duration{}};
    EXPECT_TRUE(duration.isZero());

    duration = Duration{Duration::array_t{1, 0, 0, 0, 0, 0}};
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
    // TODO compare 1y with 0y100months
    auto first{Duration{}};
    auto second{Duration{}};
    EXPECT_FALSE(first > second);
    EXPECT_FALSE(first < second);
    EXPECT_TRUE(first == second);
    EXPECT_FALSE(first != second);

    first = Duration::days(1);
    EXPECT_TRUE(first > second);
    EXPECT_TRUE(second < first);
    EXPECT_TRUE(second != first);
    EXPECT_FALSE(second == first);

    first = Duration::years(1);
    second = Duration::months(100);
    EXPECT_TRUE(second > first);
}

TEST(DurationTest, OnlyContainsUnit) {
    EXPECT_TRUE(Duration::days(5).onlyContainsUnit(unit_t::DAY));

    EXPECT_FALSE(Duration123456().onlyContainsUnit(unit_t::MONTH));

    EXPECT_FALSE(Duration().onlyContainsUnit(unit_t::MONTH));
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
