#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>

namespace test::mods {
using std::chrono::day;
using std::chrono::month;
using std::chrono::year;
using std::chrono::year_month_day;

using hbt::mods::DateTime;

using hbt::mods::Interval;

TEST(TestDateTime, DeterministicNowUsage) {
    auto today = Date::today();
    auto now = Time::now();

    auto dt{DateTime{today, now}};

    EXPECT_EQ(dt.getDate(), today);
    EXPECT_EQ(dt.getTime(), now);
}

TEST(TestDateTime, ISO8601LeapYearValid) {
    auto dt{DateTime::fromISO8601String("2024-02-29T12:00")};
    ASSERT_TRUE(dt);
    EXPECT_EQ(dt->getDaysSinceEpoch(), Date(2024, 2, 29));
}

TEST(TestDateTime, ISO8601InvalidTimeRanges) {
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T24:00"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T12:60"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T99:99"));
}

TEST(TestDateTime, ISO8601SecondsAreIgnored) {
    auto dt1{DateTime::fromISO8601String("2024-12-01T12:30:59")};
    auto dt2{DateTime::fromISO8601String("2024-12-01T12:30:00")};

    ASSERT_TRUE(dt1);
    ASSERT_TRUE(dt2);

    EXPECT_EQ(dt1->getMinutesSinceMidnight(), dt2->getMinutesSinceMidnight());
}

TEST(TestDateTime, MidnightOverflowAddition) {
    auto dt{DateTime{Date(2024, 12, 1), Time{23, 50}}};

    auto interval{Interval(Interval::Units{.minutes = 20})};

    auto res{dt + interval};

    EXPECT_EQ(res.getDate(), Date(2024, 12, 2));
    EXPECT_EQ(res.getTime(), Time(0, 10));
}

TEST(TestDateTime, LargeMinuteOverflow) {
    auto dt{DateTime{Date(2024, 12, 1), Time{10, 0}}};

    auto interval{Interval(Interval::Units{.minutes = 125})};

    auto res{dt + interval};

    EXPECT_EQ(res.getDate(), Date(2024, 12, 1));
    EXPECT_EQ(res.getTime(), Time(12, 5));
}

TEST(TestDateTime, DiffAcrossMidnight) {
    auto dt1{DateTime{Date(2024, 12, 1), Time{23, 50}}};
    auto dt2{DateTime{Date(2024, 12, 2), Time{0, 10}}};

    auto diff{DateTime::diff(dt1, dt2)};

    EXPECT_TRUE(diff == Interval(Interval::Units{.minutes = 20}));
}

TEST(TestDateTime, DiffSameTimeDifferentDates) {
    auto dt1{DateTime{Date(2024, 12, 1), Time{12, 0}}};
    auto dt2{DateTime{Date(2024, 12, 5), Time{12, 0}}};

    auto diff{DateTime::diff(dt1, dt2)};

    EXPECT_TRUE(diff == Interval(Interval::Units{.days = 4}));
}

TEST(TestDateTime, EndOfMonthAddition) {
    auto dt{DateTime{Date(2024, 1, 31), Time{10, 0}}};

    auto interval{Interval(Interval::Units{.months = 1})};

    auto res{dt + interval};

    EXPECT_TRUE(res.getDate() == Date(2024, 2, 29));
}

TEST(TestDateTime, DefaultConstructor) {
    auto dt{DateTime{}};

    EXPECT_EQ(dt.getDaysSinceEpoch(), Date::today());
    EXPECT_TRUE(dt.getMinutesSinceMidnight() == dt.getMinutesSinceMidnight());
}

TEST(TestDateTime, RoundTripVariousFormats) {
    std::vector<std::string> inputs{"2024-12-01T12:30", "2024/12/01T12:30",
                                    "2024.12.01T12:30"};

    for (const auto &input : inputs) {
        auto parsed{DateTime::fromISO8601String(input)};
        ASSERT_TRUE(parsed);

        auto serialized{parsed->toISO8601String()};
        auto reparsed{DateTime::fromISO8601String(serialized)};

        ASSERT_TRUE(reparsed);
        EXPECT_EQ(*reparsed, *parsed);
    }
}
} // namespace test::mods
