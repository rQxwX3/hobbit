#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>
#include <interval_pattern.hpp>

namespace test::mods::util {
using hbt::mods::DateTime;
using hbt::mods::Interval;

using hbt::mods::util::IntervalRecurrencePattern;

TEST(IntervalPatternTest, ZeroIntervalThrows) {
    EXPECT_THROW(IntervalRecurrencePattern(Interval{}), std::invalid_argument);
}

TEST(IntervalPatternTest, DailyInterval) {
    auto start{DateTime({2025, 1, 1})};
    auto pattern{IntervalRecurrencePattern(Interval::days(1))};

    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 2})));
    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 3})));
    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 10})));
}

TEST(IntervalPatternTest, WeeklyInterval) {
    auto start{DateTime(DateTime({2025, 1, 1}))};
    auto pattern{IntervalRecurrencePattern(Interval::weeks(1))};

    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 8})));
    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 15})));
    EXPECT_FALSE(pattern.happensOnDate(start, DateTime({2025, 1, 9})));
    EXPECT_FALSE(pattern.happensOnDate(start, DateTime({2025, 1, 14})));
}

TEST(IntervalPatternTest, ThreeDayInterval) {
    auto start{DateTime(DateTime({2025, 1, 1}))};
    auto pattern{IntervalRecurrencePattern(Interval::days(3))};

    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 4})));
    EXPECT_TRUE(pattern.happensOnDate(start, DateTime({2025, 1, 7})));
    EXPECT_FALSE(pattern.happensOnDate(start, DateTime({2025, 1, 5})));
    EXPECT_FALSE(pattern.happensOnDate(start, DateTime({2025, 1, 6})));
}

TEST(IntervalPatternTest, GetOccurrencesProducesCorrectSequence) {
    auto start{DateTime(DateTime({2025, 1, 1}))};
    auto pattern{IntervalRecurrencePattern(Interval::days(2))};

    auto result = pattern.getOccurrencesOfDate(start, DateTime({2025, 1, 7}));

    ASSERT_GE(result.size(), 1u);

    EXPECT_EQ(result[0], DateTime({2025, 1, 7}));

    for (size_t i = 1; i < result.size(); ++i) {
        EXPECT_LT(result[i - 1], result[i]);
    }
}

TEST(IntervalPatternTest, GetOccurrencesEmptyWhenNoMatch) {
    auto start = DateTime(DateTime({2025, 1, 1}));
    auto pattern = IntervalRecurrencePattern(Interval::days(2));

    auto result = pattern.getOccurrencesOfDate(start, DateTime({2025, 1, 3}));

    EXPECT_TRUE(result.empty());
}

TEST(IntervalPatternTest, JSONRoundTripPreservesInterval) {
    auto pattern = IntervalRecurrencePattern(Interval::days(5));

    auto json = pattern.toJSON();
    auto restored = IntervalRecurrencePattern::fromJSON(json);

    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored->getInterval(), pattern.getInterval());
}
} // namespace test::mods::util
