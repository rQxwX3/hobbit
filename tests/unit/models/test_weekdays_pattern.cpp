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

TEST(WeekdaysRecurrencePattern, ThrowsOnNonWeeklyIntervals) {
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
}

TEST(WeekdayRecurrencePatternTest, ThrowsOnEmptyWeek) {
    EXPECT_THROW(
        WeekdaysRecurrencePattern(DateTime::now(), Week(), Interval::weeks(1)),
        std::invalid_argument);
}

TEST(WeekdayRecurrencePatternTest, MatchesOnlyCorrectWeekdays) {
    auto start{DateTime({2025, 1, 6})}; // monday

    auto pattern{WeekdaysRecurrencePattern(
        start, Week({Week::Weekday::WEDNESDAY}), Interval::weeks(1))};

    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 6})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 7})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 8})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 9})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 10})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 11})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 12})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 13})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 14})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 15})));
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

    auto json = pattern.toJSON();
    auto restored{WeekdaysRecurrencePattern::fromJSON(json)};

    ASSERT_TRUE(restored);
    EXPECT_EQ(pattern, restored);
}
} // namespace test::mods::util
