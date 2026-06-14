#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>
#include <recurrence_pattern.hpp>
#include <week.hpp>
#include <weekdays_pattern.hpp>

namespace test::mods::util {
using hbt::mods::DateTime;
using hbt::mods::Interval;
using hbt::mods::Week;

using hbt::mods::util::WeekdaysRecurrencePattern;

TEST(WeekdaysRecurrencePatternTest, ThrowsOnNonWeeklyIntervals) {
    EXPECT_THROW(WeekdaysRecurrencePattern(DateTime::now(),
                                           Week({Week::Weekday::MONDAY}),
                                           Interval::minutes(1)),
                 std::invalid_argument);

    EXPECT_THROW(WeekdaysRecurrencePattern(DateTime::now(),
                                           Week({Week::Weekday::MONDAY}),
                                           Interval::hours(1)),
                 std::invalid_argument);

    EXPECT_THROW(WeekdaysRecurrencePattern(DateTime::now(),
                                           Week({Week::Weekday::MONDAY}),
                                           Interval::days(1)),
                 std::invalid_argument);

    EXPECT_THROW(WeekdaysRecurrencePattern(DateTime::now(),
                                           Week({Week::Weekday::MONDAY}),
                                           Interval::months(1)),
                 std::invalid_argument);

    EXPECT_THROW(WeekdaysRecurrencePattern(DateTime::now(),
                                           Week({Week::Weekday::MONDAY}),
                                           Interval::years(1)),
                 std::invalid_argument);

    /* mods::clndr::Week ctor */
    EXPECT_THROW(WeekdaysRecurrencePattern(
                     hbt::mods::clndr::Week(DateTime::now()),
                     Week({Week::Weekday::MONDAY}), Interval::years(1)),
                 std::invalid_argument);
}

TEST(WeekdayRecurrencePatternTest, ThrowsOnEmptyWeek) {
    /* mods::DateTime ctor */
    EXPECT_THROW(
        WeekdaysRecurrencePattern(DateTime::now(), Week(), Interval::weeks(1)),
        std::invalid_argument);

    /* mods::clndr::Week ctor */
    EXPECT_THROW(
        WeekdaysRecurrencePattern(hbt::mods::clndr::Week(DateTime::now()),
                                  Week(), Interval::weeks(1)),
        std::invalid_argument);
}

TEST(WeekdayRecurrencePatternTest, HappensOnDate) {
    auto start{DateTime({2025, 1, 6})}; // monday

    auto pattern{WeekdaysRecurrencePattern(
        start, Week({Week::Weekday::WEDNESDAY, Week::Weekday::FRIDAY}),
        Interval::weeks(1))};

    EXPECT_FALSE(
        pattern.happensOnDate(DateTime({2025, 1, 1}))); // wednesday in past
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 6})));  // monday
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 7})));  // tuesday
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 8})));   // wednesday
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 9})));  // thursday
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 10})));  // friday
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 11}))); // saturday
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 12}))); // sunday
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 13}))); // monday
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 14}))); // tuesday
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 15})));  // wednesday
}

TEST(WeekdayRecurrencePatternTest, IntervalAffectsWeekdayRepetition) {
    auto start{DateTime({2025, 1, 6})}; // monday

    auto pattern{WeekdaysRecurrencePattern(
        start, Week({Week::Weekday::WEDNESDAY}), Interval::weeks(2))};

    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 8})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 15})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 22})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 29})));
}

TEST(WeekdayRecurrencePatternTest, GetOccurrencesSingleMatch) {
    auto start{DateTime({2025, 1, 6})}; // monday

    auto pattern{WeekdaysRecurrencePattern(
        start, Week({Week::Weekday::WEDNESDAY}), Interval::weeks(1))};

    auto result{pattern.getOccurrencesOfDate(DateTime({2025, 1, 8}))};
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result.front(), DateTime({2025, 1, 8}));
}

TEST(WeekdayRecurrencePatternTest, GetOccurrencesEmptyWhenNoMatch) {
    auto start{DateTime({2025, 1, 6})}; // monday

    auto pattern{WeekdaysRecurrencePattern(
        start, Week({Week::Weekday::WEDNESDAY}), Interval::weeks(1))};

    ASSERT_TRUE(pattern.getOccurrencesOfDate(start).empty());
}

TEST(WeekdayRecurrencePatternTest, JSONRoundTrip) {
    auto pattern{WeekdaysRecurrencePattern(
        DateTime::now(), Week({Week::Weekday::WEDNESDAY}), Interval::weeks(1))};

    auto json = WeekdaysRecurrencePattern::JSON::encode(pattern);
    auto restored{WeekdaysRecurrencePattern::JSON::decode(json)};

    ASSERT_TRUE(restored);
    EXPECT_EQ(pattern, restored);
}

TEST(WeekdaysRecurrencePatternTest, FromJSONFailsOnInvalidJSON) {
    /* empty json */
    auto json = nlohmann::json{};
    auto result{WeekdaysRecurrencePattern::JSON::decode(json)};
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              WeekdaysRecurrencePattern::JSON::Error::MissingRequiredField);

    /* missing first week */
    json = {{WeekdaysRecurrencePattern::JSON::weekField, Week().toJSON()},
            {WeekdaysRecurrencePattern::JSON::intervalField,
             Interval::weeks(1).toJSON()}};
    result = WeekdaysRecurrencePattern::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              WeekdaysRecurrencePattern::JSON::Error::MissingRequiredField);

    /* missing interval */
    json = {{WeekdaysRecurrencePattern::JSON::weekField, Week().toJSON()},
            {WeekdaysRecurrencePattern::JSON::firstCalendarWeekField,
             hbt::mods::clndr::Week(DateTime()).toJSON()}};
    result = WeekdaysRecurrencePattern::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              WeekdaysRecurrencePattern::JSON::Error::MissingRequiredField);

    /* missing week */
    json = {{WeekdaysRecurrencePattern::JSON::intervalField,
             Interval::weeks(1).toJSON()},
            {WeekdaysRecurrencePattern::JSON::firstCalendarWeekField,
             hbt::mods::clndr::Week(DateTime()).toJSON()}};
    result = WeekdaysRecurrencePattern::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              WeekdaysRecurrencePattern::JSON::Error::MissingRequiredField);

    auto week{Week()};
    week.addWeekday(Week::Weekday::FRIDAY);

    /* invalid interval */
    json = {{WeekdaysRecurrencePattern::JSON::intervalField,
             Interval::months(1).toJSON()}, // month, not weekly
            {WeekdaysRecurrencePattern::JSON::firstCalendarWeekField,
             hbt::mods::clndr::Week(DateTime()).toJSON()},
            {WeekdaysRecurrencePattern::JSON::weekField, week.toJSON()}};
    result = WeekdaysRecurrencePattern::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              WeekdaysRecurrencePattern::JSON::Error::FailedToValidateInterval);

    /* invalid week */
    json = {{WeekdaysRecurrencePattern::JSON::intervalField,
             Interval::weeks(1).toJSON()},
            {WeekdaysRecurrencePattern::JSON::weekField,
             Week().toJSON()}, // empty week
            {WeekdaysRecurrencePattern::JSON::firstCalendarWeekField,
             hbt::mods::clndr::Week(DateTime()).toJSON()}};
    result = WeekdaysRecurrencePattern::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              WeekdaysRecurrencePattern::JSON::Error::FailedToValidateWeek);
}
} // namespace test::mods::util
