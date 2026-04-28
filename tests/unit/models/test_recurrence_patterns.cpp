#include <gtest/gtest.h>

#include <date.hpp>
#include <datetime.hpp>
#include <duration.hpp>
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
using hbt::mods::Duration;
using hbt::mods::Interval;
using hbt::mods::Weekdays;

using hbt::mods::util::IntervalRecurrencePattern;
using hbt::mods::util::WeekdayRecurrencePattern;

TEST(IntervalRecurrencePatternTest, ZeroIntervalThrows) {
    EXPECT_THROW(IntervalRecurrencePattern(Interval{}), std::invalid_argument);
}

TEST(IntervalRecurrencePatternTest, DailyIntervalHappensSequentially) {
    auto start = DateTime(Date(year(2025), month(1), day(1)));
    auto pattern = IntervalRecurrencePattern(Interval(Duration::days(1)));

    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(2))));
    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(3))));
    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(10))));
}

TEST(IntervalRecurrencePatternTest, WeeklyIntervalAlignment) {
    auto start = DateTime(Date(year(2025), month(1), day(1)));
    auto pattern = IntervalRecurrencePattern(Interval(Duration::days(7)));

    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(8))));
    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(15))));
    EXPECT_FALSE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(9))));
    EXPECT_FALSE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(14))));
}

TEST(IntervalRecurrencePatternTest, NonAlignedIntervalSkipsCorrectly) {
    auto start = DateTime(Date(year(2025), month(1), day(1)));

    auto pattern = IntervalRecurrencePattern(Interval(Duration::days(3)));

    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(4))));
    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(7))));
    EXPECT_FALSE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(5))));
    EXPECT_FALSE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(6))));
}

TEST(IntervalRecurrencePatternTest, GetOccurrencesProducesCorrectSequence) {
    auto start = DateTime(Date(year(2025), month(1), day(1)));
    auto pattern = IntervalRecurrencePattern(Interval(Duration::days(2)));

    auto result =
        pattern.getOccurrencesOnDate(start, Date(year(2025), month(1), day(7)));

    ASSERT_GE(result.size(), 1u);

    EXPECT_EQ(result[0].getDate(), Date(year(2025), month(1), day(7)));

    for (size_t i = 1; i < result.size(); ++i) {
        EXPECT_LT(result[i - 1].getDate(), result[i].getDate());
    }
}

TEST(IntervalRecurrencePatternTest, GetOccurrencesEmptyWhenNoMatch) {
    auto start = DateTime(Date(year(2025), month(1), day(1)));
    auto pattern = IntervalRecurrencePattern(Interval(Duration::days(2)));

    auto result =
        pattern.getOccurrencesOnDate(start, Date(year(2025), month(1), day(3)));

    EXPECT_TRUE(result.empty());
}

TEST(IntervalRecurrencePatternTest, JSONRoundTripPreservesInterval) {
    auto pattern = IntervalRecurrencePattern(Interval(Duration::days(5)));

    auto json = pattern.toJSON();
    auto restored = IntervalRecurrencePattern::fromJSON(json);

    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored->getInterval().getDuration(),
              pattern.getInterval().getDuration());
}

TEST(WeekdayRecurrencePatternTest, EmptyWeekdaysThrows) {
    Weekdays empty{Weekdays::days_t{}};

    EXPECT_THROW(WeekdayRecurrencePattern(Interval(Duration::days(7)), empty),
                 std::invalid_argument);
}

TEST(WeekdayRecurrencePatternTest, MatchesOnlyCorrectWeekday) {
    auto start = DateTime(Date(year(2025), month(1), day(6))); // monday

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval(Duration::days(7)), w);

    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(8))));
    EXPECT_FALSE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(7))));
}

TEST(WeekdayRecurrencePatternTest, IntervalAffectsWeekdayRepetition) {
    auto start = DateTime(Date(year(2025), month(1), day(6)));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval(Duration::days(14)), w);

    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(8))));
    EXPECT_TRUE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(22))));
}

TEST(WeekdayRecurrencePatternTest, NoFalsePositiveOnWrongWeekday) {
    auto start = DateTime(Date(year(2025), month(1), day(6)));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval(Duration::days(7)), w);

    EXPECT_FALSE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(9))));
    EXPECT_FALSE(
        pattern.happensOnDate(start, Date(year(2025), month(1), day(10))));
}

TEST(WeekdayRecurrencePatternTest, GetOccurrencesSingleMatch) {
    auto start = DateTime(Date(year(2025), month(1), day(6)));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval(Duration::days(7)), w);

    auto result =
        pattern.getOccurrencesOnDate(start, Date(year(2025), month(1), day(8)));

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result.front().getDate(), Date(year(2025), month(1), day(8)));
}

TEST(WeekdayRecurrencePatternTest, GetOccurrencesEmptyWhenNoMatch) {
    auto start = DateTime(Date(year(2025), month(1), day(6)));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval(Duration::days(7)), w);

    auto result =
        pattern.getOccurrencesOnDate(start, Date(year(2025), month(1), day(7)));

    EXPECT_TRUE(result.empty());
}

TEST(WeekdayRecurrencePatternTest, JSONRoundTripPreservesState) {
    Weekdays w{{DateTime::weekday_t::WEDNESDAY, DateTime::weekday_t::FRIDAY}};

    auto pattern = WeekdayRecurrencePattern(Interval(Duration::days(7)), w);

    auto json = pattern.toJSON();
    auto restored = WeekdayRecurrencePattern::fromJSON(json);

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getInterval().getDuration(),
              pattern.getInterval().getDuration());

    EXPECT_EQ(restored->getWeekdays().getDays(),
              pattern.getWeekdays().getDays());
}
} // namespace test::mods::util
