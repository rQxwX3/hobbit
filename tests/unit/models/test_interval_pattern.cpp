#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>
#include <interval_pattern.hpp>

namespace test::mods::util {
using hbt::mods::DateTime;
using hbt::mods::Interval;

using hbt::mods::util::IntervalRecurrencePattern;

TEST(IntervalPatternTest, ZeroIntervalThrows) {
    EXPECT_THROW(IntervalRecurrencePattern(DateTime::now(), Interval{}),
                 std::invalid_argument);
}

TEST(IntervalPatternTest, HappensOnDate) {
    auto start{DateTime({2025, 1, 1})};

    /* every minute = every day */
    auto pattern{IntervalRecurrencePattern(start, Interval::minutes(1))};
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 1})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2026, 3, 12})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2031, 6, 28})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2000, 6, 28})));

    /* every hour = every day */
    pattern = IntervalRecurrencePattern(start, Interval::hours(1));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 1})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2026, 3, 12})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2031, 6, 28})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2000, 6, 28})));

    /* daily interval */
    pattern = IntervalRecurrencePattern(start, Interval::days(1));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 1})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 2})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 10})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2000, 6, 28})));

    /* weekly interval */
    pattern = IntervalRecurrencePattern(start, Interval::weeks(1));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 1})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 8})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 15})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 2})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2000, 6, 28})));

    /* monthly interval */
    pattern = IntervalRecurrencePattern(start, Interval::months(1));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 1})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 2, 1})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 3, 1})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 2})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2000, 6, 28})));

    /* annual interval */
    pattern = IntervalRecurrencePattern(start, Interval::years(1));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2025, 1, 1})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2026, 1, 1})));
    EXPECT_TRUE(pattern.happensOnDate(DateTime({2027, 1, 1})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 2})));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2000, 6, 28})));

    /* false before start same day */
    start = DateTime({2025, 1, 1}, {12, 0});
    pattern = IntervalRecurrencePattern(start, Interval::minutes(1));
    EXPECT_FALSE(pattern.happensOnDate(DateTime({2025, 1, 1}, {11, 59})));
}

TEST(IntervalPatternTest, GetFirstOccurrenceOfDate) {
    /* every minute => first moment after start */
    auto start{DateTime({2025, 1, 1})};
    auto pattern{IntervalRecurrencePattern(start, Interval::minutes(1))};
    EXPECT_EQ(pattern.getFirstOccurrenceOfDate(start),
              DateTime({2025, 1, 1}, {0, 0}));

    start = DateTime({2025, 1, 1}, {12, 12});
    pattern = IntervalRecurrencePattern(start, Interval::minutes(1));
    EXPECT_EQ(pattern.getFirstOccurrenceOfDate(start),
              DateTime({2025, 1, 1}, {12, 12}));

    /* mixed interval */
    start = DateTime({2025, 1, 1}, {12, 12});
    pattern =
        IntervalRecurrencePattern(start, Interval({.years = 1, .minutes = 1}));
    EXPECT_EQ(pattern.getFirstOccurrenceOfDate(DateTime({2026, 1, 1})),
              DateTime({2026, 1, 1}, {12, 13}));
}

TEST(IntervalPatternTest, GetFirstOccurrenceOfDateEmptyWhenNoOccurrences) {
    /* no occurrences in the past */
    auto start{DateTime({2025, 1, 1})};
    auto pattern{IntervalRecurrencePattern(start, Interval::weeks(1))};
    EXPECT_FALSE(
        pattern.getFirstOccurrenceOfDate(DateTime({2024, 1, 2})).has_value());

    start = DateTime({2025, 1, 1});
    pattern = IntervalRecurrencePattern(start, Interval::minutes(1));
    EXPECT_FALSE(
        pattern.getFirstOccurrenceOfDate(DateTime({2024, 1, 1})).has_value());
}

TEST(IntervalPatternTest, GetOccurrencesOfDate) {
    /* every hour => 24 occurrences a day */
    auto start{DateTime({2025, 1, 1})};
    auto pattern{IntervalRecurrencePattern(start, Interval::hours(1))};
    auto occurrencesOfDate{pattern.getOccurrencesOfDate(start)};
    ASSERT_EQ(occurrencesOfDate.size(), 24);
    EXPECT_EQ(occurrencesOfDate[0], start);
    EXPECT_EQ(occurrencesOfDate[23], DateTime(start.getDate(), {23, 00}));

    /* every hour starting from 1am => 23 occurrences on the start date */
    start = DateTime({2025, 1, 1}, {1, 0});
    pattern = IntervalRecurrencePattern(start, Interval::hours(1));
    occurrencesOfDate = pattern.getOccurrencesOfDate(start);
    ASSERT_EQ(occurrencesOfDate.size(), 23);
    EXPECT_EQ(occurrencesOfDate[0], start);

    /* daily occurrences => 1 occurrence a day */
    start = DateTime({2025, 1, 1});
    pattern = IntervalRecurrencePattern(start, Interval::days(1));
    occurrencesOfDate = pattern.getOccurrencesOfDate(start);
    ASSERT_EQ(occurrencesOfDate.size(), 1);
    EXPECT_EQ(occurrencesOfDate[0], start);

    /* subday interval */
    start = DateTime({2025, 1, 1}, {0, 0});
    pattern = IntervalRecurrencePattern(start, Interval::hours(5));
    occurrencesOfDate = pattern.getOccurrencesOfDate(start);
    ASSERT_EQ(occurrencesOfDate.size(), 5);
    EXPECT_EQ(occurrencesOfDate[0], DateTime({2025, 1, 1}, {0, 0}));
    EXPECT_EQ(occurrencesOfDate[1], DateTime({2025, 1, 1}, {5, 0}));
    EXPECT_EQ(occurrencesOfDate[2], DateTime({2025, 1, 1}, {10, 0}));
    EXPECT_EQ(occurrencesOfDate[3], DateTime({2025, 1, 1}, {15, 0}));
    EXPECT_EQ(occurrencesOfDate[4], DateTime({2025, 1, 1}, {20, 0}));

    /* stops at midnight boundary */
    start = DateTime({2025, 1, 1}, {23, 0});
    pattern = IntervalRecurrencePattern(start, Interval::hours(2));
    occurrencesOfDate = pattern.getOccurrencesOfDate(start);
    ASSERT_EQ(occurrencesOfDate.size(), 1);
    EXPECT_EQ(occurrencesOfDate[0], DateTime({2025, 1, 1}, {23, 0}));
}

TEST(IntervalPatternTest, GetOccurrencesOfDateEmptyWhenNoOccurences) {
    /* tomorrow doesn't align with weekly interval */
    auto start{DateTime({2025, 1, 1})};
    auto pattern{IntervalRecurrencePattern(start, Interval::weeks(1))};
    auto occurrencesOfDate{
        pattern.getOccurrencesOfDate(DateTime({2025, 1, 2}))};
    EXPECT_TRUE(occurrencesOfDate.empty());

    /* no occurrences in the past */
    start = DateTime({2025, 1, 1});
    pattern = IntervalRecurrencePattern(start, Interval::weeks(1));
    occurrencesOfDate = pattern.getOccurrencesOfDate(DateTime({2024, 1, 2}));
    EXPECT_TRUE(occurrencesOfDate.empty());
}

TEST(IntervalPatternTest, JSONRoundTrip) {
    auto pattern{IntervalRecurrencePattern(DateTime({2025, 1, 1}, {12, 12}),
                                           Interval::weeks(1))};

    EXPECT_EQ(IntervalRecurrencePattern::fromJSON(pattern.toJSON()).value(),
              pattern);
}

TEST(IntervalPatternTest, FromJSONFailsOnInvalidJSON) {
    /* empty json */
    auto json = nlohmann::json{};

    EXPECT_FALSE(IntervalRecurrencePattern::fromJSON(json));

    /* missing start */
    json = {{"interval", Interval::days(10).toJSON()}};
    EXPECT_FALSE(IntervalRecurrencePattern::fromJSON(json));

    /* invalid start */
    json = {{"start", "invalid"}, {"interval", Interval::days(10).toJSON()}};
    EXPECT_FALSE(IntervalRecurrencePattern::fromJSON(json));

    /* missing interval */
    json = {{"start", DateTime::now().toISO8601String()}};
    EXPECT_FALSE(IntervalRecurrencePattern::fromJSON(json));

    /* invalid interval */
    json = {{"start", DateTime::now().toISO8601String()},
            {"interval", "invalid"}};
    EXPECT_FALSE(IntervalRecurrencePattern::fromJSON(json));

    /* invalid interval (zero-interval) */
    json = {{"start", DateTime::now().toISO8601String()},
            {"interval", Interval{}.toJSON()}};
    EXPECT_FALSE(IntervalRecurrencePattern::fromJSON(json));
}
} // namespace test::mods::util
