#include <gtest/gtest.h>

#include <duration_units.hpp>
#include <duration_units_parser.hpp>

namespace test::mods::util {
using hbt::mods::DurationUnits;
using hbt::mods::util::DurationUnitsParser;

TEST(DurationUnitsParserTest, ReturnsNulloptOnInvalidNaturalLanguage) {
    auto input{""}; // empty string
    EXPECT_FALSE(DurationUnitsParser::fromNaturalLanguage(input).has_value());

    input = "hello"; // not a unit
    EXPECT_FALSE(DurationUnitsParser::fromNaturalLanguage(input).has_value());

    input = "1234";
    EXPECT_FALSE(DurationUnitsParser::fromNaturalLanguage(input).has_value());

    input = "!@#$%^&&*()";
    EXPECT_FALSE(DurationUnitsParser::fromNaturalLanguage(input).has_value());

    input = "1yesterday";
    EXPECT_FALSE(DurationUnitsParser::fromNaturalLanguage(input).has_value());

    input = "year"; // unit w/o value
    EXPECT_FALSE(DurationUnitsParser::fromNaturalLanguage(input).has_value());

    input = "year1"; // unit-value pair in wrong order
    EXPECT_FALSE(DurationUnitsParser::fromNaturalLanguage(input).has_value());

    auto input2{std::to_string(DurationUnits::maxValue + 1) +
                "years"}; // value out-of-bounds
    EXPECT_FALSE(DurationUnitsParser::fromNaturalLanguage(input2).has_value());
}
} // namespace test::mods::util
