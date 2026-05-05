#include <gtest/gtest.h>

#include <date.hpp>
#include <datetime.hpp>
#include <interval.hpp>
#include <recurrence_patterns.hpp>
#include <weekdays.hpp>

#include <chrono>

namespace test::mods::util {
using hbt::mods::Date;
using year = std::chrono::year;
using month = std::chrono::month;
using day = std::chrono::day;

using hbt::mods::DateTime;
using hbt::mods::Interval;
using hbt::mods::Weekdays;

using hbt::mods::util::IntervalRecurrencePattern;
using hbt::mods::util::WeekdayRecurrencePattern;

TEST(IntervalRecurrencePatternTest, ZeroIntervalThrows) {
    EXPECT_THROW(IntervalRecurrencePattern(Interval{}), std::invalid_argument);
}

TEST(IntervalRecurrencePatternTest, DailyIntervalHappensSequentially) {
    auto start = DateTime(Date(2025, 1, 1));
    auto pattern = IntervalRecurrencePattern(Interval::days(1));

    EXPECT_TRUE(pattern.happensOnDate(start, Date(2025, 1, 2)));
    EXPECT_TRUE(pattern.happensOnDate(start, Date(2025, 1, 3)));
    EXPECT_TRUE(pattern.happensOnDate(start, Date(2025, 1, 10)));
}

TEST(IntervalRecurrencePatternTest, WeeklyIntervalAlignment) {
    auto start = DateTime(Date(2025, 1, 1));
    auto pattern = IntervalRecurrencePattern(Interval::days(7));

    EXPECT_TRUE(pattern.happensOnDate(start, Date(2025, 1, 8)));
    EXPECT_TRUE(pattern.happensOnDate(start, Date(2025, 1, 15)));
    EXPECT_FALSE(pattern.happensOnDate(start, Date(2025, 1, 9)));
    EXPECT_FALSE(pattern.happensOnDate(start, Date(2025, 1, 14)));
}

TEST(IntervalRecurrencePatternTest, NonAlignedIntervalSkipsCorrectly) {
    auto start = DateTime(Date(2025, 1, 1));

    auto pattern = IntervalRecurrencePattern(Interval::days(3));

    EXPECT_TRUE(pattern.happensOnDate(start, Date(2025, 1, 4)));
    EXPECT_TRUE(pattern.happensOnDate(start, Date(2025, 1, 7)));
    EXPECT_FALSE(pattern.happensOnDate(start, Date(2025, 1, 5)));
    EXPECT_FALSE(pattern.happensOnDate(start, Date(2025, 1, 6)));
}

TEST(IntervalRecurrencePatternTest, GetOccurrencesProducesCorrectSequence) {
    auto start = DateTime(Date(2025, 1, 1));
    auto pattern = IntervalRecurrencePattern(Interval::days(2));

    auto result = pattern.getTimeStampsOnDate(start, Date(2025, 1, 7));

    ASSERT_GE(result.size(), 1u);

    EXPECT_EQ(result[0].getDays(), Date(2025, 1, 7));

    for (size_t i = 1; i < result.size(); ++i) {
        EXPECT_LT(result[i - 1].getDays(), result[i].getDays());
    }
}

TEST(IntervalRecurrencePatternTest, GetOccurrencesEmptyWhenNoMatch) {
    auto start = DateTime(Date(2025, 1, 1));
    auto pattern = IntervalRecurrencePattern(Interval::days(2));

    auto result = pattern.getTimeStampsOnDate(start, Date(2025, 1, 3));

    EXPECT_TRUE(result.empty());
}

TEST(IntervalRecurrencePatternTest, JSONRoundTripPreservesInterval) {
    auto pattern = IntervalRecurrencePattern(Interval::days(5));

    auto json = pattern.toJSON();
    auto restored = IntervalRecurrencePattern::fromJSON(json);

    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored->getInterval(), pattern.getInterval());
}

TEST(WeekdayRecurrencePatternTest, EmptyWeekdaysThrows) {
    Weekdays empty{Weekdays::days_t{}};

    EXPECT_THROW(WeekdayRecurrencePattern(Interval::days(7), empty),
                 std::invalid_argument);
}

TEST(WeekdayRecurrencePatternTest, MatchesOnlyCorrectWeekday) {
    auto start = DateTime(Date(2025, 1, 6)); // monday

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(7), w);

    EXPECT_TRUE(pattern.happensOnDate(start, Date(2025, 1, 8)));
    EXPECT_FALSE(pattern.happensOnDate(start, Date(2025, 1, 7)));
}

TEST(WeekdayRecurrencePatternTest, IntervalAffectsWeekdayRepetition) {
    auto start = DateTime(Date(2025, 1, 6));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(14), w);

    EXPECT_TRUE(pattern.happensOnDate(start, Date(2025, 1, 8)));
    EXPECT_TRUE(pattern.happensOnDate(start, Date((2025), 1, 22)));
}

TEST(WeekdayRecurrencePatternTest, NoFalsePositiveOnWrongWeekday) {
    auto start = DateTime(Date(2025, 1, 6));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(7), w);

    EXPECT_FALSE(pattern.happensOnDate(start, Date(2025, 1, 9)));
    EXPECT_FALSE(pattern.happensOnDate(start, Date(2025, 1, 10)));
}

TEST(WeekdayRecurrencePatternTest, GetOccurrencesSingleMatch) {
    auto start = DateTime(Date(2025, 1, 6));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(7), w);

    auto result = pattern.getTimeStampsOnDate(start, Date(2025, 1, 8));

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result.front().getDays(), Date(2025, 1, 8));
}

TEST(WeekdayRecurrencePatternTest, GetOccurrencesEmptyWhenNoMatch) {
    auto start = DateTime(Date(2025, 1, 6));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(7), w);

    auto result = pattern.getTimeStampsOnDate(start, Date(2025, 1, 7));

    EXPECT_TRUE(result.empty());
}

TEST(WeekdayRecurrencePatternTest, JSONRoundTripPreservesState) {
    Weekdays w{{DateTime::weekday_t::WEDNESDAY, DateTime::weekday_t::FRIDAY}};

    auto pattern = WeekdayRecurrencePattern(Interval::days(7), w);

    auto json = pattern.toJSON();
    auto restored = WeekdayRecurrencePattern::fromJSON(json);

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getInterval(), pattern.getInterval());

    EXPECT_EQ(restored->getWeekdays().getDays(),
              pattern.getWeekdays().getDays());
}
} // namespace test::mods::util
