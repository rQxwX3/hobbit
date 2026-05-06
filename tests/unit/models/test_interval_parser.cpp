#include <gtest/gtest.h>

#include <interval.hpp>

namespace test::mods::util {
using hbt::mods::Interval;

TEST(NaturalLanguageParserTest, ParsesValidInputs) {
    auto result{Interval::fromNaturalLanguage(
        "1year 2months 3weeks 4days 5hours 6minutes")};
    ASSERT_TRUE(result);

    EXPECT_EQ(result->getUnitValue(Interval::Unit::YEAR), 1);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::MONTH), 2);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::WEEK), 3);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::DAY), 4);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::HOUR), 5);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::MINUTE), 6);

    result = Interval::fromNaturalLanguage("1y2m3w4d5h6min");
    ASSERT_TRUE(result);

    result = Interval::fromNaturalLanguage("1Y 2MoNtHs 3W 4D 5H 6MiN");
    ASSERT_TRUE(result);
}

TEST(NaturalLanguageParserTest, HandlesFiltering) {
    auto result{Interval::fromNaturalLanguage("!@# 1 year !! 2 months @@")};
    ASSERT_TRUE(result);

    EXPECT_EQ(result->getUnitValue(Interval::Unit::YEAR), 1);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::MONTH), 2);
}
TEST(NaturalLanguageParserTest, RejectsInvalidInputs) {
    EXPECT_FALSE(Interval::fromNaturalLanguage(""));
    EXPECT_FALSE(Interval::fromNaturalLanguage("hello"));
    EXPECT_FALSE(Interval::fromNaturalLanguage("1234"));
    EXPECT_FALSE(Interval::fromNaturalLanguage("!@#$%^&*()"));

    EXPECT_FALSE(Interval::fromNaturalLanguage("year"));
    EXPECT_FALSE(Interval::fromNaturalLanguage("year1"));
    EXPECT_FALSE(Interval::fromNaturalLanguage("1yesterday"));

    EXPECT_FALSE(Interval::fromNaturalLanguage("1.5year"));
    EXPECT_FALSE(Interval::fromNaturalLanguage("1/5year"));
}

TEST(NaturalLanguageParserTest, RejectsDuplicateUnits) {
    EXPECT_FALSE(Interval::fromNaturalLanguage("1year 2year").has_value());
    EXPECT_FALSE(Interval::fromNaturalLanguage("1y2y").has_value());
}

TEST(NaturalLanguageParserTest, RejectsOutOfBoundsValues) {
    auto input{std::to_string(Interval::maxValue + 1) + "year"};
    EXPECT_FALSE(Interval::fromNaturalLanguage(input).has_value());
}

TEST(NaturalLanguageParserTest, Formatting) {
    Interval i{Interval::Units{.years = 1,
                               .months = 2,
                               .weeks = 0,
                               .days = 0,
                               .hours = 0,
                               .minutes = 0}};
    auto formatted{i.toNaturalLanguage()};

    EXPECT_EQ(formatted, "1year, 2months");

    i = Interval::years(1);
    formatted = i.toNaturalLanguage();
    EXPECT_EQ(formatted, "1year"); // singular

    i = Interval{};
    formatted = i.toNaturalLanguage();
    EXPECT_EQ(formatted, ""); // no units
}

TEST(ISO8601ParserTest, ParsesValidInputs) {
    auto result{Interval::fromISO8601String("PT0M")};
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->isZero());

    result = Interval::fromISO8601String("PT1M");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getUnitValue(Interval::Unit::MINUTE), 1);

    result = Interval::fromISO8601String("PT1H1M");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getUnitValue(Interval::Unit::HOUR), 1);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::MINUTE), 1);

    result = Interval::fromISO8601String("P1Y2M3W4DT5H6M");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getUnitValue(Interval::Unit::YEAR), 1);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::MONTH), 2);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::WEEK), 3);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::DAY), 4);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::HOUR), 5);
    EXPECT_EQ(result->getUnitValue(Interval::Unit::MINUTE), 6);
}

TEST(ISO8601ParserTest, RejectsInvalidInputs) {
    EXPECT_FALSE(Interval::fromISO8601String("").has_value());
    EXPECT_FALSE(Interval::fromISO8601String("PT").has_value());
    EXPECT_FALSE(Interval::fromISO8601String("1Y").has_value());
    EXPECT_FALSE(Interval::fromISO8601String("P1Y1H").has_value());
    EXPECT_FALSE(Interval::fromISO8601String("P1M1Y").has_value());
    EXPECT_FALSE(Interval::fromISO8601String("P1YT1M1H").has_value());
}

TEST(ISO8601ParserTest, Formatting) {
    Interval i{};

    EXPECT_EQ(i.toISO8601String(), "PT0M");

    i = Interval::minutes(1);
    EXPECT_EQ(i.toISO8601String(), "PT1M");

    i = Interval::hours(1) + Interval::minutes(1);
    EXPECT_EQ(i.toISO8601String(), "PT1H1M");

    i = Interval::years(1) + Interval::months(2);
    EXPECT_EQ(i.toISO8601String(), "P1Y2M");
}
} // namespace test::mods::util
