#include <gtest/gtest.h>

#include <duration.hpp>

namespace test::mods::util {

using hbt::mods::Duration;

TEST(NaturalLanguageParserTest, ParsesValidInputs) {
    auto result{Duration::fromNaturalLanguage(
        "1year 2months 3weeks 4days 5hours 6minutes")};
    ASSERT_TRUE(result);

    EXPECT_EQ(result->getUnitValue(Duration::unit_t::YEAR), 1);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::MONTH), 2);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::WEEK), 3);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::DAY), 4);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::HOUR), 5);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::MINUTE), 6);

    result = Duration::fromNaturalLanguage("1y2m3w4d5h6min");
    ASSERT_TRUE(result);

    result = Duration::fromNaturalLanguage("1Y 2MoNtHs 3W 4D 5H 6MiN");
    ASSERT_TRUE(result);
}

TEST(NaturalLanguageParserTest, HandlesFiltering) {
    auto result{Duration::fromNaturalLanguage("!@# 1 year !! 2 months @@")};
    ASSERT_TRUE(result);

    EXPECT_EQ(result->getUnitValue(Duration::unit_t::YEAR), 1);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::MONTH), 2);
}
TEST(NaturalLanguageParserTest, RejectsInvalidInputs) {
    EXPECT_FALSE(Duration::fromNaturalLanguage(""));
    EXPECT_FALSE(Duration::fromNaturalLanguage("hello"));
    EXPECT_FALSE(Duration::fromNaturalLanguage("1234"));
    EXPECT_FALSE(Duration::fromNaturalLanguage("!@#$%^&*()"));

    EXPECT_FALSE(Duration::fromNaturalLanguage("year"));
    EXPECT_FALSE(Duration::fromNaturalLanguage("year1"));
    EXPECT_FALSE(Duration::fromNaturalLanguage("1yesterday"));

    EXPECT_FALSE(Duration::fromNaturalLanguage("1.5year"));
    EXPECT_FALSE(Duration::fromNaturalLanguage("1/5year"));
}

TEST(NaturalLanguageParserTest, RejectsDuplicateUnits) {
    EXPECT_FALSE(Duration::fromNaturalLanguage("1year 2year").has_value());
    EXPECT_FALSE(Duration::fromNaturalLanguage("1y2y").has_value());
}

TEST(NaturalLanguageParserTest, RejectsOutOfBoundsValues) {
    auto input{std::to_string(Duration::maxValue + 1) + "year"};
    EXPECT_FALSE(Duration::fromNaturalLanguage(input).has_value());
}

TEST(NaturalLanguageParserTest, Formatting) {
    Duration d{Duration::Units{.years = 1,
                               .months = 2,
                               .weeks = 0,
                               .days = 0,
                               .hours = 0,
                               .minutes = 0}};
    auto formatted{d.toNaturalLanguage()};

    EXPECT_EQ(formatted, "1year, 2months");

    d = Duration::years(1);
    formatted = d.toNaturalLanguage();
    EXPECT_EQ(formatted, "1year"); // singular

    d = Duration{};
    formatted = d.toNaturalLanguage();
    EXPECT_EQ(formatted, ""); // no units
}

TEST(ISO8601ParserTest, ParsesValidInputs) {
    auto result{Duration::fromISO8601String("PT0M")};
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->isZero());

    result = Duration::fromISO8601String("PT1M");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::MINUTE), 1);

    result = Duration::fromISO8601String("PT1H1M");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::HOUR), 1);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::MINUTE), 1);

    result = Duration::fromISO8601String("P1Y2M3W4DT5H6M");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::YEAR), 1);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::MONTH), 2);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::WEEK), 3);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::DAY), 4);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::HOUR), 5);
    EXPECT_EQ(result->getUnitValue(Duration::unit_t::MINUTE), 6);
}

TEST(ISO8601ParserTest, RejectsInvalidInputs) {
    EXPECT_FALSE(Duration::fromISO8601String("").has_value());
    EXPECT_FALSE(Duration::fromISO8601String("PT").has_value());
    EXPECT_FALSE(Duration::fromISO8601String("1Y").has_value());
    EXPECT_FALSE(Duration::fromISO8601String("P1Y1H").has_value());
    EXPECT_FALSE(Duration::fromISO8601String("P1M1Y").has_value());
    EXPECT_FALSE(Duration::fromISO8601String("P1YT1M1H").has_value());
}

TEST(ISO8601ParserTest, Formatting) {
    Duration d{};

    EXPECT_EQ(d.toISO8601String(), "PT0M");

    d = Duration::minutes(1);
    EXPECT_EQ(d.toISO8601String(), "PT1M");

    d = Duration::hours(1) + Duration::minutes(1);
    EXPECT_EQ(d.toISO8601String(), "PT1H1M");

    d = Duration::years(1) + Duration::months(2);
    EXPECT_EQ(d.toISO8601String(), "P1Y2M");
}
} // namespace test::mods::util
