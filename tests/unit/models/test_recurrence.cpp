#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>
#include <recurrence.hpp>

namespace test::mods::util {
using hbt::mods::DateTime;
using hbt::mods::Interval;
using hbt::mods::Week;
using hbt::mods::util::IntervalRecurrencePattern;
using hbt::mods::util::Recurrence;
using hbt::mods::util::WeekdaysRecurrencePattern;

TEST(RecurrenceTest, GetPatternType) {
    /* IntervalRecurrencePattern */
    auto recurrence{Recurrence(
        IntervalRecurrencePattern(DateTime::now(), Interval::weeks(1)))};

    EXPECT_EQ(recurrence.getPatternType(), Recurrence::PatternType::Interval);

    /* WeekdaysRecurrencePattern */
    auto week{Week()};
    week.addWeekday(Week::Weekday::FRIDAY);
    recurrence = Recurrence(
        WeekdaysRecurrencePattern(DateTime::now(), week, Interval::weeks(1)));

    EXPECT_EQ(recurrence.getPatternType(), Recurrence::PatternType::Weekday);
}

TEST(RecurrenceTest, GetPattern) {
    /* IntervalRecurrencePattern */
    auto intervalPattern{
        IntervalRecurrencePattern(DateTime::now(), Interval::weeks(1))};
    auto recurrence{Recurrence(intervalPattern)};

    EXPECT_EQ(recurrence.getIntervalPattern(), intervalPattern);
    EXPECT_THROW(recurrence.getWeekdayPattern(), std::bad_variant_access);

    /* WeekdaysRecurrencePattern */
    auto week{Week()};
    week.addWeekday(Week::Weekday::FRIDAY);

    auto weekdaysPattern{
        WeekdaysRecurrencePattern(DateTime::now(), week, Interval::weeks(1))};
    recurrence = Recurrence(intervalPattern);

    EXPECT_EQ(recurrence.getWeekdayPattern(), weekdaysPattern);
    EXPECT_THROW(recurrence.getIntervalPattern(), std::bad_variant_access);
}

TEST(RecurrenceTest, JSONRoundTrip) {
    /* IntervalRecurrencePattern */
    auto original{Recurrence(
        IntervalRecurrencePattern(DateTime::now(), Interval::weeks(1)))};

    auto json = Recurrence::JSON::encode(original);
    auto restored{Recurrence::JSON::decode(json)};

    ASSERT_TRUE(restored);
    EXPECT_EQ(restored.value(), original);

    /* WeekdaysRecurrencePattern */
    auto week{Week()};
    week.addWeekday(Week::Weekday::FRIDAY);
    original = Recurrence(
        WeekdaysRecurrencePattern(DateTime::now(), week, Interval::weeks(1)));

    json = Recurrence::JSON::encode(original);
    restored = Recurrence::JSON::decode(json);

    ASSERT_TRUE(restored);
    EXPECT_EQ(restored.value(), original);
}

TEST(RecurrenceTest, FromJSONFailsOnInvalidJSON) {
    /* empty json */
    auto json = nlohmann::json{};
    auto result{Recurrence::JSON::decode(json)};
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Recurrence::JSON::Error::MissingRequiredField);

    auto week{Week()};
    week.addWeekday(Week::Weekday::FRIDAY);
    auto weekdaysPattern{
        WeekdaysRecurrencePattern(DateTime::now(), week, Interval::weeks(1))};

    /* missing pattern type */
    json = {{
        "pattern",
        WeekdaysRecurrencePattern::JSON::encode(weekdaysPattern),
    }};
    result = Recurrence::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Recurrence::JSON::Error::MissingRequiredField);

    /* missing pattern */
    json = {{
        "type",
        Recurrence::JSON::weekdayPatternTypeValue,
    }};
    result = Recurrence::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Recurrence::JSON::Error::MissingRequiredField);

    /* invalid pattern type */
    json = {{"type", ""},
            {
                "pattern",
                WeekdaysRecurrencePattern::JSON::encode(weekdaysPattern),
            }};
    result = Recurrence::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Recurrence::JSON::Error::UnsupportedPatternType);

    /* invalid WeekdaysRecurrencePattern */
    json = {{"type", "weekday"}, {"pattern", ""}};
    result = Recurrence::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              Recurrence::JSON::Error::FailedToParseWeekdayPattern);

    /* invalid IntervalRecurrencePattern */
    json = {{"type", "interval"}, {"pattern", ""}};
    result = Recurrence::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              Recurrence::JSON::Error::FailedToParseIntervalPattern);
}
} // namespace test::mods::util
