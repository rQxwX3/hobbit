#include <gtest/gtest.h>

#include <duration_units.hpp>

namespace test::mods {
using hbt::mods::DurationUnits;
using unit_t = hbt::mods::DurationUnits::unit_t;

TEST(DurationUnitsTest, EmptyOnConstruction) {
    auto durationUnits{DurationUnits{}};

    ASSERT_EQ(durationUnits.getUnit(unit_t::YEAR), 0);
    ASSERT_EQ(durationUnits.getUnit(unit_t::MONTH), 0);
    ASSERT_EQ(durationUnits.getUnit(unit_t::WEEK), 0);
    ASSERT_EQ(durationUnits.getUnit(unit_t::DAY), 0);
    ASSERT_EQ(durationUnits.getUnit(unit_t::HOUR), 0);
    ASSERT_EQ(durationUnits.getUnit(unit_t::MINUTE), 0);
}

TEST(DurationUnitsTest, ArrayConstructor) {
    auto durationUnits{DurationUnits{DurationUnits::array_t{1, 2, 3, 4, 5, 6}}};

    ASSERT_EQ(durationUnits.getUnit(unit_t::YEAR), 1);
    ASSERT_EQ(durationUnits.getUnit(unit_t::MONTH), 2);
    ASSERT_EQ(durationUnits.getUnit(unit_t::WEEK), 3);
    ASSERT_EQ(durationUnits.getUnit(unit_t::DAY), 4);
    ASSERT_EQ(durationUnits.getUnit(unit_t::HOUR), 5);
    ASSERT_EQ(durationUnits.getUnit(unit_t::MINUTE), 6);
}

TEST(DurationUnitsTest, PerUnitConstructor) {
    auto durationUnits{DurationUnits{{.years = 1,
                                      .months = 2,
                                      .weeks = 3,
                                      .days = 4,
                                      .hours = 5,
                                      .minutes = 6}}};

    ASSERT_EQ(durationUnits.getUnit(unit_t::YEAR), 1);
    ASSERT_EQ(durationUnits.getUnit(unit_t::MONTH), 2);
    ASSERT_EQ(durationUnits.getUnit(unit_t::WEEK), 3);
    ASSERT_EQ(durationUnits.getUnit(unit_t::DAY), 4);
    ASSERT_EQ(durationUnits.getUnit(unit_t::HOUR), 5);
    ASSERT_EQ(durationUnits.getUnit(unit_t::MINUTE), 6);
}

TEST(DurationUnitsTest, IsZero) {
    auto durationUnits{DurationUnits{}};
    EXPECT_TRUE(durationUnits.isZero());

    durationUnits.addUnit(unit_t::DAY, 1);
    EXPECT_FALSE(durationUnits.isZero());
}

TEST(DurationUnitsTest, AddGetUnits) {
    auto durationUnits{DurationUnits{}};

    durationUnits.addUnit(unit_t::YEAR, 1);
    EXPECT_EQ(durationUnits.getUnit(unit_t::YEAR), 1);

    durationUnits.addUnit(unit_t::MONTH, 1);
    EXPECT_EQ(durationUnits.getUnit(unit_t::MONTH), 1);

    durationUnits.addUnit(unit_t::WEEK, 1);
    EXPECT_EQ(durationUnits.getUnit(unit_t::WEEK), 1);

    durationUnits.addUnit(unit_t::DAY, 1);
    EXPECT_EQ(durationUnits.getUnit(unit_t::DAY), 1);

    durationUnits.addUnit(unit_t::HOUR, 1);
    EXPECT_EQ(durationUnits.getUnit(unit_t::HOUR), 1);

    durationUnits.addUnit(unit_t::MINUTE, 1);
    EXPECT_EQ(durationUnits.getUnit(unit_t::MINUTE), 1);
}

TEST(DurationUnitsTest, OperatorPlus) {
    auto first{DurationUnits{}};
    first.addUnit(unit_t::YEAR, 1);
    first.addUnit(unit_t::MONTH, 1);
    first.addUnit(unit_t::WEEK, 1);
    first.addUnit(unit_t::DAY, 1);
    first.addUnit(unit_t::HOUR, 1);
    first.addUnit(unit_t::MINUTE, 1);

    auto second{DurationUnits{}};
    second.addUnit(unit_t::YEAR, 1);
    second.addUnit(unit_t::MONTH, 2);
    second.addUnit(unit_t::WEEK, 3);
    second.addUnit(unit_t::DAY, 4);
    second.addUnit(unit_t::HOUR, 5);
    second.addUnit(unit_t::MINUTE, 6);

    auto result{first + second};
    EXPECT_EQ(result.getUnit(unit_t::YEAR), 2);
    EXPECT_EQ(result.getUnit(unit_t::MONTH), 3);
    EXPECT_EQ(result.getUnit(unit_t::WEEK), 4);
    EXPECT_EQ(result.getUnit(unit_t::DAY), 5);
    EXPECT_EQ(result.getUnit(unit_t::HOUR), 6);
    EXPECT_EQ(result.getUnit(unit_t::MINUTE), 7);
}

TEST(DurationUnitsTest, ComparisonOperators) {
    auto first{DurationUnits{}};
    auto second{DurationUnits{}};
    EXPECT_TRUE(first == second);
    EXPECT_TRUE(second == first);

    first.addUnit(unit_t::DAY, 1);
    EXPECT_TRUE(first > second);
    EXPECT_TRUE(second < first);

    second.addUnit(unit_t::DAY, 2);
    EXPECT_TRUE(second > first);
    EXPECT_TRUE(first < second);

    first.addUnit(unit_t::MONTH, 1);
    EXPECT_TRUE(first > second);
    EXPECT_TRUE(second < first);

    second.addUnit(unit_t::MONTH, 1);
    first.addUnit(unit_t::DAY, 1);
    EXPECT_TRUE(first == second);
    EXPECT_TRUE(second == first);
}

TEST(DurationUnitsTest, ToFromValidISO8601String) {
    auto first{DurationUnits{}};
    auto firstISO{first.toISO8601String()};
    EXPECT_EQ(firstISO, "PT0M");
    auto restored{DurationUnits::fromISO8601String(firstISO)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addUnit(unit_t::MINUTE, 1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "PT1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addUnit(unit_t::HOUR, 1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "PT1H1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addUnit(unit_t::DAY, 1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1DT1H1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addUnit(unit_t::WEEK, 1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1W1DT1H1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addUnit(unit_t::MONTH, 1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1M1W1DT1H1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addUnit(unit_t::YEAR, 1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1Y1M1W1DT1H1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);
}

TEST(DurationUnitsTest, ToFromInValidISO8601String) {
    // invalid empty duration
    auto invalidISO{DurationUnits::fromISO8601String("PT")};
    ASSERT_TRUE(!invalidISO.has_value());

    invalidISO = DurationUnits::fromISO8601String("");
    ASSERT_TRUE(!invalidISO.has_value());

    // no global prefix
    invalidISO = DurationUnits::fromISO8601String("1Y");
    ASSERT_TRUE(!invalidISO.has_value());

    // no time section designator
    invalidISO = DurationUnits::fromISO8601String("P1Y1H");
    ASSERT_TRUE(!invalidISO.has_value());

    // incorrect order in date section
    invalidISO = DurationUnits::fromISO8601String("P1M1Y");
    ASSERT_TRUE(!invalidISO.has_value());
    // incorrect order in time section
    invalidISO = DurationUnits::fromISO8601String("P1YT1M1H");
    ASSERT_TRUE(!invalidISO.has_value());

    // number without designator
    invalidISO = DurationUnits::fromISO8601String("P1");
    ASSERT_TRUE(!invalidISO.has_value());

    invalidISO = DurationUnits::fromISO8601String("P1YT1");
    ASSERT_TRUE(!invalidISO.has_value());

    // designator without number
    invalidISO = DurationUnits::fromISO8601String("PY");
    ASSERT_TRUE(!invalidISO.has_value());

    invalidISO = DurationUnits::fromISO8601String("P1YTH");
    ASSERT_TRUE(!invalidISO.has_value());

    // trailing number
    invalidISO = DurationUnits::fromISO8601String("P1Y1");
    ASSERT_TRUE(!invalidISO.has_value());

    // trailing designator
    invalidISO = DurationUnits::fromISO8601String("P1YM");
    ASSERT_TRUE(!invalidISO.has_value());

    // invalid designator in date section
    invalidISO = DurationUnits::fromISO8601String("P1Y1Z");
    ASSERT_TRUE(!invalidISO.has_value());

    // invalid designator in time section
    invalidISO = DurationUnits::fromISO8601String("P1Y1T1H1Z");
    ASSERT_TRUE(!invalidISO.has_value());
}

TEST(DurationUnitsTest, FromValidNaturalLanguage) {
    using hbt::mods::DurationUnits;

    auto input{"1year, 2months, 3weeks, 4 days, 5 hours, 6 minutes"};
    auto parsed{DurationUnits::fromNaturalLanguage(input)};
    ASSERT_TRUE(parsed.has_value());
    auto durationUnits{DurationUnits{DurationUnits::Units{.years = 1,
                                                          .months = 2,
                                                          .weeks = 3,
                                                          .days = 4,
                                                          .hours = 5,
                                                          .minutes = 6}}};
    EXPECT_EQ(parsed.value(), durationUnits);

    input = "1y2m3w4d5h6min";
    parsed = DurationUnits::fromNaturalLanguage(input);
    ASSERT_TRUE(parsed.has_value());
    durationUnits = DurationUnits{DurationUnits::Units{.years = 1,
                                                       .months = 2,
                                                       .weeks = 3,
                                                       .days = 4,
                                                       .hours = 5,
                                                       .minutes = 6}};
    EXPECT_EQ(parsed.value(), durationUnits);

    input = "1Y2mOnTh3wk4day5hr6min";
    parsed = DurationUnits::fromNaturalLanguage(input);
    ASSERT_TRUE(parsed.has_value());
    durationUnits = DurationUnits{DurationUnits::Units{.years = 1,
                                                       .months = 2,
                                                       .weeks = 3,
                                                       .days = 4,
                                                       .hours = 5,
                                                       .minutes = 6}};
    EXPECT_EQ(parsed.value(), durationUnits);

    input = "1year";
    parsed = DurationUnits::fromNaturalLanguage(input);
    ASSERT_TRUE(parsed.has_value());
    durationUnits = DurationUnits{DurationUnits::Units{.years = 1,
                                                       .months = 0,
                                                       .weeks = 0,
                                                       .days = 0,
                                                       .hours = 0,
                                                       .minutes = 0}};
    EXPECT_EQ(parsed.value(), durationUnits);

    // minus of a negative value is stripped as a separator
    input = "-1y";
    parsed = DurationUnits::fromNaturalLanguage(input);
    ASSERT_TRUE(parsed.has_value());
    durationUnits = DurationUnits{DurationUnits::Units{.years = 1,
                                                       .months = 0,
                                                       .weeks = 0,
                                                       .days = 0,
                                                       .hours = 0,
                                                       .minutes = 0}};
    EXPECT_EQ(parsed.value(), durationUnits);

    auto maxInput{std::to_string(DurationUnits::maxValue) + "year"};
    parsed = DurationUnits::fromNaturalLanguage(maxInput);
    ASSERT_TRUE(parsed.has_value());
    durationUnits =
        DurationUnits{DurationUnits::Units{.years = DurationUnits::maxValue,
                                           .months = 0,
                                           .weeks = 0,
                                           .days = 0,
                                           .hours = 0,
                                           .minutes = 0}};
    EXPECT_EQ(parsed.value(), durationUnits);

    input = "!@#$%^&*()_+|/<>,.~      1 !@#$%^&*()_+|/<>,.~`     years    "
            "!@#$%^&*()_+|/<>,.~` 2 !@#$%^&*()_+|/<>,.~`      months   "
            "!@#$%^&*()_+|/<>,.~` ";
    parsed = DurationUnits::fromNaturalLanguage(input);
    ASSERT_TRUE(parsed.has_value());
    durationUnits = DurationUnits{DurationUnits::Units{.years = 1,
                                                       .months = 2,
                                                       .weeks = 0,
                                                       .days = 0,
                                                       .hours = 0,
                                                       .minutes = 0}};
    EXPECT_EQ(parsed.value(), durationUnits);

    input = "6min5h4d3w2m1y";
    parsed = DurationUnits::fromNaturalLanguage(input);
    ASSERT_TRUE(parsed.has_value());
    durationUnits = DurationUnits{DurationUnits::Units{.years = 1,
                                                       .months = 2,
                                                       .weeks = 3,
                                                       .days = 4,
                                                       .hours = 5,
                                                       .minutes = 6}};
    EXPECT_EQ(parsed.value(), durationUnits);

    input = "6min1y";
    parsed = DurationUnits::fromNaturalLanguage(input);
    ASSERT_TRUE(parsed.has_value());
    durationUnits = DurationUnits{DurationUnits::Units{.years = 1,
                                                       .months = 0,
                                                       .weeks = 0,
                                                       .days = 0,
                                                       .hours = 0,
                                                       .minutes = 6}};
    EXPECT_EQ(parsed.value(), durationUnits);
}

TEST(DurationUnitsTest, FromInvalidNaturalLanguage) {
    auto input{""};
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "year";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "nothing";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1 nothing";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "!@#$%^&&*()";
    EXPECT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "year 1";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1 yera";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1yesterday";
    EXPECT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1 monht";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1 weke";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1 dya";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1 huor";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1 mintue";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "year 1month";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1 year month";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1.5year";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1/5year";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1y2mo3w4d5h6m7m";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1year 2months 3weeks 4days 5hours 6minutes 7minutes";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    input = "1y1y";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());

    auto input1{std::to_string(DurationUnits::maxValue + 1) + "y"};
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input1).has_value());

    auto input2{"1y" + std::to_string(DurationUnits::maxValue + 1) + "m"};
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input2).has_value());
}
}; // namespace test::mods
