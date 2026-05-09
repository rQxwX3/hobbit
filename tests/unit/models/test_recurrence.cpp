#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>
#include <recurrence.hpp>
#include <weekdays.hpp>


TEST(WeekdayRecurrencePatternTest, EmptyWeekdaysThrows) {
    Weekdays empty{Weekdays::days_t{}};

    EXPECT_THROW(WeekdayRecurrencePattern(Interval::days(7), empty),
                 std::invalid_argument);
}

TEST(WeekdayRecurrencePatternTest, MatchesOnlyCorrectWeekday) {
    auto start = DateTime(DateTime({2025, 1, 6})); // monday

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(7), w);

    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 8})));
    EXPECT_FALSE(pattern.happensOnDate(start, DateTime({2025, 1, 7})));
}

TEST(WeekdayRecurrencePatternTest, IntervalAffectsWeekdayRepetition) {
    auto start = DateTime(DateTime({2025, 1, 6}));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(14), w);

    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 8})));
    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 22})));
}

TEST(WeekdayRecurrencePatternTest, NoFalsePositiveOnWrongWeekday) {
    auto start = DateTime(DateTime({2025, 1, 6}));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(7), w);

    EXPECT_FALSE(pattern.happensOnDate(start, DateTime({2025, 1, 9})));
    EXPECT_FALSE(pattern.happensOnDate(start, DateTime({2025, 1, 10})));
}

TEST(WeekdayRecurrencePatternTest, GetOccurrencesSingleMatch) {
    auto start = DateTime(DateTime({2025, 1, 6}));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(7), w);

    auto result = pattern.getTimeStampsOnDate(start, DateTime({2025, 1, 8}));

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result.front(), DateTime({2025, 1, 8}));
}

TEST(WeekdayRecurrencePatternTest, GetOccurrencesEmptyWhenNoMatch) {
    auto start = DateTime(DateTime({2025, 1, 6}));

    Weekdays w{{DateTime::weekday_t::WEDNESDAY}};
    auto pattern = WeekdayRecurrencePattern(Interval::days(7), w);

    auto result = pattern.getTimeStampsOnDate(start, DateTime({2025, 1, 7}));

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
