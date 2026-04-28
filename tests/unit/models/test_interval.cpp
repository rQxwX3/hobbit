#include <gtest/gtest.h>

#include <interval.hpp>

namespace test::mods {
using hbt::mods::Duration;
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

TEST(IntervalTest, IsZero) {
    auto interval{Interval{}};
    EXPECT_TRUE(interval.isZero());

    auto nonZero{Interval{Duration::years(1)}};
    EXPECT_FALSE(nonZero.isZero());
}

TEST(IntervalTest, OperatorPlus) {
    auto first{Interval{Duration::years(1)}};
    auto second{Interval{Duration::months(2)}};

    auto result{first + second};

    EXPECT_EQ(result.getUnitValue(unit_t::YEAR), 1);
    EXPECT_EQ(result.getUnitValue(unit_t::MONTH), 2);
}

TEST(IntervalTest, OperatorPlusResetsMonthHandling) {
    auto first{Interval{Duration::years(1), Interval::MonthHandling::CUT_OFF}};
    auto second{
        Interval{Duration::months(1), Interval::MonthHandling::WRAP_AROUND}};

    auto result{first + second};

    EXPECT_EQ(result.getMonthHandling(), Interval::defaultMonthHandling);
}

TEST(IntervalTest, OnlyContainsUnit) {
    auto interval{Interval{Duration::days(5)}};

    EXPECT_TRUE(interval.onlyContainsUnit(unit_t::DAY));
    EXPECT_FALSE(interval.onlyContainsUnit(unit_t::MONTH));
}

TEST(IntervalTest, ToFromJSON) {
    auto original{Interval{Duration::years(1) + Duration::months(1) +
                               Duration::weeks(1) + Duration::days(1) +
                               Duration::hours(1) + Duration::minutes(1),
                           Interval::MonthHandling::CUT_OFF}};

    auto json = original.toJSON();
    auto restored{Interval::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getDuration(), original.getDuration());
    EXPECT_EQ(restored->getMonthHandling(), original.getMonthHandling());
}

TEST(IntervalTest, FromJSONInvalid) {
    nlohmann::json json{};
    EXPECT_FALSE(Interval::fromJSON(json).has_value());

    json = {{Interval::jsonDurationField, "invalid"},
            {Interval::jsonMonthHandlingField, "0"}};
    EXPECT_FALSE(Interval::fromJSON(json).has_value());
}
} // namespace test::mods
