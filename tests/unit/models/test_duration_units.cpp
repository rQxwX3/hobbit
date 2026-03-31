#include <gtest/gtest.h>

#include <duration_units.hpp>

namespace test::mods {
using hbt::mods::DurationUnits;

TEST(DurationUnitsTest, EmptyOnConstruction) {
    auto durationUnits{DurationUnits{}};

    ASSERT_EQ(durationUnits.getYears(), 0);
    ASSERT_EQ(durationUnits.getMonths(), 0);
    ASSERT_EQ(durationUnits.getWeeks(), 0);
    ASSERT_EQ(durationUnits.getDays(), 0);
    ASSERT_EQ(durationUnits.getHours(), 0);
    ASSERT_EQ(durationUnits.getMinutes(), 0);
}

TEST(DurationUnitsTest, ArrayConstructor) {
    auto durationUnits{DurationUnits{DurationUnits::array_t{1, 2, 3, 4, 5, 6}}};

    ASSERT_EQ(durationUnits.getYears(), 1);
    ASSERT_EQ(durationUnits.getMonths(), 2);
    ASSERT_EQ(durationUnits.getWeeks(), 3);
    ASSERT_EQ(durationUnits.getDays(), 4);
    ASSERT_EQ(durationUnits.getHours(), 5);
    ASSERT_EQ(durationUnits.getMinutes(), 6);
}

TEST(DurationUnitsTest, PerUnitConstructor) {
    auto durationUnits{DurationUnits{{.years = 1,
                                      .months = 2,
                                      .weeks = 3,
                                      .days = 4,
                                      .hours = 5,
                                      .minutes = 6}}};

    ASSERT_EQ(durationUnits.getYears(), 1);
    ASSERT_EQ(durationUnits.getMonths(), 2);
    ASSERT_EQ(durationUnits.getWeeks(), 3);
    ASSERT_EQ(durationUnits.getDays(), 4);
    ASSERT_EQ(durationUnits.getHours(), 5);
    ASSERT_EQ(durationUnits.getMinutes(), 6);
}

TEST(DurationUnitsTest, IsZero) {
    auto durationUnits{DurationUnits{}};
    EXPECT_TRUE(durationUnits.isZero());

    durationUnits.addDays(1);
    EXPECT_FALSE(durationUnits.isZero());
}

TEST(DurationUnitsTest, AddGetUnits) {
    auto durationUnits{DurationUnits{}};

    durationUnits.addYears(1);
    EXPECT_EQ(durationUnits.getYears(), 1);

    durationUnits.addMonths(1);
    EXPECT_EQ(durationUnits.getMonths(), 1);

    durationUnits.addWeeks(1);
    EXPECT_EQ(durationUnits.getWeeks(), 1);

    durationUnits.addDays(1);
    EXPECT_EQ(durationUnits.getDays(), 1);

    durationUnits.addHours(1);
    EXPECT_EQ(durationUnits.getHours(), 1);

    durationUnits.addMinutes(1);
    EXPECT_EQ(durationUnits.getMinutes(), 1);
}

TEST(DurationUnitsTest, AddAutoChangesUnits) {
    auto monthsToYears1{DurationUnits{}};
    monthsToYears1.addMonths(DurationUnits::monthsInYear);
    EXPECT_EQ(monthsToYears1.getMonths(), 0);
    EXPECT_EQ(monthsToYears1.getYears(), 1);

    auto monthsToYears2{DurationUnits{}};
    monthsToYears2.addMonths(DurationUnits::monthsInYear * 2);
    EXPECT_EQ(monthsToYears2.getMonths(), 0);
    EXPECT_EQ(monthsToYears2.getYears(), 2);

    auto monthsToYears3{DurationUnits{}};
    monthsToYears3.addMonths(DurationUnits::monthsInYear + 1);
    EXPECT_EQ(monthsToYears3.getMonths(), 1);
    EXPECT_EQ(monthsToYears3.getYears(), 1);

    auto weeksToMonths1{DurationUnits{}};
    weeksToMonths1.addWeeks(DurationUnits::weeksInMonth);
    EXPECT_EQ(weeksToMonths1.getWeeks(), 0);
    EXPECT_EQ(weeksToMonths1.getMonths(), 1);

    auto weeksToMonths2{DurationUnits{}};
    weeksToMonths2.addWeeks(DurationUnits::weeksInMonth * 3);
    EXPECT_EQ(weeksToMonths2.getWeeks(), 0);
    EXPECT_EQ(weeksToMonths2.getMonths(), 3);

    auto weeksToMonths3{DurationUnits{}};
    weeksToMonths3.addWeeks(DurationUnits::weeksInMonth + 2);
    EXPECT_EQ(weeksToMonths3.getWeeks(), 2);
    EXPECT_EQ(weeksToMonths3.getMonths(), 1);

    auto daysToWeek1{DurationUnits{}};
    daysToWeek1.addDays(DurationUnits::daysInWeek);
    EXPECT_EQ(daysToWeek1.getDays(), 0);
    EXPECT_EQ(daysToWeek1.getWeeks(), 1);

    auto daysToWeek2{DurationUnits{}};
    daysToWeek2.addDays(DurationUnits::daysInWeek * 3);
    EXPECT_EQ(daysToWeek2.getDays(), 0);
    EXPECT_EQ(daysToWeek2.getWeeks(), 3);

    auto daysToWeek3{DurationUnits{}};
    daysToWeek3.addDays(DurationUnits::daysInWeek + 4);
    EXPECT_EQ(daysToWeek3.getDays(), 4);
    EXPECT_EQ(daysToWeek3.getWeeks(), 1);

    auto hoursToDays1{DurationUnits{}};
    hoursToDays1.addHours(DurationUnits::hoursInDay);
    EXPECT_EQ(hoursToDays1.getHours(), 0);
    EXPECT_EQ(hoursToDays1.getDays(), 1);

    auto hoursToDays2{DurationUnits{}};
    hoursToDays2.addHours(DurationUnits::hoursInDay * 5);
    EXPECT_EQ(hoursToDays2.getHours(), 0);
    EXPECT_EQ(hoursToDays2.getDays(), 5);

    auto hoursToDays3{DurationUnits{}};
    hoursToDays3.addHours(DurationUnits::hoursInDay + 15);
    EXPECT_EQ(hoursToDays3.getHours(), 15);
    EXPECT_EQ(hoursToDays3.getDays(), 1);

    auto minutesToHours1{DurationUnits{}};
    minutesToHours1.addMinutes(DurationUnits::minutesInHour);
    EXPECT_EQ(minutesToHours1.getMinutes(), 0);
    EXPECT_EQ(minutesToHours1.getHours(), 1);

    auto minutesToHours2{DurationUnits{}};
    minutesToHours2.addMinutes(DurationUnits::minutesInHour * 6);
    EXPECT_EQ(minutesToHours2.getMinutes(), 0);
    EXPECT_EQ(minutesToHours2.getHours(), 6);

    auto minutesToHours3{DurationUnits{}};
    minutesToHours3.addMinutes(DurationUnits::minutesInHour + 30);
    EXPECT_EQ(minutesToHours3.getMinutes(), 30);
    EXPECT_EQ(minutesToHours3.getHours(), 1);
}

TEST(DurationUnitsTest, OperatorPlus) {
    auto first{DurationUnits{}};
    first.addYears(1);
    first.addMonths(1);
    first.addWeeks(1);
    first.addDays(1);
    first.addHours(1);
    first.addMinutes(1);

    auto second{DurationUnits{}};
    second.addYears(1);
    second.addMonths(2);
    second.addWeeks(3);
    second.addDays(4);
    second.addHours(5);
    second.addMinutes(6);

    auto result{first + second};
    EXPECT_EQ(result.getYears(), 2);
    EXPECT_EQ(result.getMonths(), 3);
    EXPECT_EQ(result.getWeeks(), 4);
    EXPECT_EQ(result.getDays(), 5);
    EXPECT_EQ(result.getHours(), 6);
    EXPECT_EQ(result.getMinutes(), 7);
}

TEST(DurationUnitsTest, ComparisonOperators) {
    auto first{DurationUnits{}};
    auto second{DurationUnits{}};
    EXPECT_TRUE(first == second);
    EXPECT_TRUE(second == first);

    first.addDays(1);
    EXPECT_TRUE(first > second);
    EXPECT_TRUE(second < first);

    second.addDays(2);
    EXPECT_TRUE(second > first);
    EXPECT_TRUE(first < second);

    first.addMonths(1);
    EXPECT_TRUE(first > second);
    EXPECT_TRUE(second < first);

    second.addMonths(1);
    first.addDays(1);
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

    first.addMinutes(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "PT1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addHours(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "PT1H1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addDays(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1DT1H1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addWeeks(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1W1DT1H1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addMonths(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1M1W1DT1H1M");
    restored = DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addYears(1);
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

    input = "1y2mo3w4d5h6m";
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

    input = "6m5h4d3w2mo1y";
    parsed = DurationUnits::fromNaturalLanguage(input);
    ASSERT_TRUE(parsed.has_value());
    durationUnits = DurationUnits{DurationUnits::Units{.years = 1,
                                                       .months = 2,
                                                       .weeks = 3,
                                                       .days = 4,
                                                       .hours = 5,
                                                       .minutes = 6}};
    EXPECT_EQ(parsed.value(), durationUnits);

    input = "6m1y";
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

    input = "1 nothing";
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

    input = "1y1y";
    ASSERT_FALSE(DurationUnits::fromNaturalLanguage(input).has_value());
}
}; // namespace test::mods
