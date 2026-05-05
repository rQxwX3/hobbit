#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>

namespace test::mods {
using hbt::mods::DateTime;
using hbt::mods::Interval;

using Date = DateTime::Date;
using Time = DateTime::Time;

TEST(TestDateTime, ConstructsFromValidDate) {
    EXPECT_NO_THROW(DateTime({2026, 5, 5}));

    EXPECT_NO_THROW(DateTime({1000, 12, 31}));

    EXPECT_NO_THROW(DateTime({-1000, 12, 31}));

    EXPECT_NO_THROW(DateTime({1, 1, 1}));

    EXPECT_NO_THROW(DateTime({2024, 2, 29})); // leap year
}

TEST(TestDateTime, ConstructsFromValidDateAndTime) {
    EXPECT_NO_THROW(DateTime({2026, 5, 5}, {23, 59}));

    EXPECT_NO_THROW(DateTime({1000, 12, 31}, {0, 0}));

    EXPECT_NO_THROW(DateTime({-1000, 12, 31}, {12, 12}));

    EXPECT_NO_THROW(DateTime({1, 1, 1}, {1, 1}));

    EXPECT_NO_THROW(DateTime({2024, 2, 29}, {0, 30})); // leap year
}

TEST(TestDateTime, ThrowsOnInvalidDate) {
    EXPECT_THROW(DateTime({2023, 2, 29}),
                 std::invalid_argument); // not a leap year

    EXPECT_THROW(DateTime({2023, 3, 32}), std::invalid_argument);

    EXPECT_THROW(DateTime({2023, 3, 0}), std::invalid_argument);

    EXPECT_THROW(DateTime({2023, 0, 13}), std::invalid_argument);

    EXPECT_THROW(DateTime({2023, 0, 0}), std::invalid_argument);

    EXPECT_THROW(DateTime({2023, 0, 0}), std::invalid_argument);
}

TEST(TestDateTime, ThrowsOnInvalidDateAndTime) {
    EXPECT_THROW(DateTime({2026, 5, 5}, {24, 0}), std::invalid_argument);

    EXPECT_THROW(DateTime({2026, 5, 5}, {23, 60}), std::invalid_argument);

    EXPECT_THROW(DateTime({2026, 5, 5}, {24, 60}), std::invalid_argument);
}

TEST(TestDateTime, DefaultConstructorCreatesNow) {
    EXPECT_EQ(DateTime(), DateTime::now());
}

TEST(TestDateTime, GetDate) {
    EXPECT_EQ(DateTime({2026, 5, 5}).getDate(), Date({2026, 5, 5}));
    EXPECT_EQ(DateTime({2026, 5, 5}, {12, 12}).getDate(), Date({2026, 5, 5}));

    EXPECT_EQ(DateTime({1000, 12, 31}).getDate(), Date({1000, 12, 31}));

    EXPECT_EQ(DateTime({-1000, 12, 31}).getDate(), Date({-1000, 12, 31}));

    EXPECT_EQ(DateTime({2024, 2, 29}).getDate(),
              Date({2024, 2, 29})); // leap year
}

TEST(TestDateTime, GetTime) {
    EXPECT_EQ(DateTime({2026, 5, 5}, {12, 12}).getTime(), Time({12, 12}));

    EXPECT_EQ(DateTime({2026, 5, 5}, {00, 00}).getTime(), Time({0, 0}));

    EXPECT_EQ(DateTime({2026, 5, 5}, {23, 59}).getTime(), Time({23, 59}));
}

TEST(TestDateTime, GetWeekday) {
    using weekday_t = DateTime::weekday_t;

    EXPECT_EQ(DateTime({2026, 5, 3}, {12, 12}).getWeekday(), weekday_t::SUNDAY);
    EXPECT_EQ(DateTime({2026, 5, 4}, {12, 12}).getWeekday(), weekday_t::MONDAY);
    EXPECT_EQ(DateTime({2026, 5, 5}, {12, 12}).getWeekday(),
              weekday_t::TUESDAY);
    EXPECT_EQ(DateTime({2026, 5, 6}, {12, 12}).getWeekday(),
              weekday_t::WEDNESDAY);
    EXPECT_EQ(DateTime({2026, 5, 7}, {12, 12}).getWeekday(),
              weekday_t::THURSDAY);
    EXPECT_EQ(DateTime({2026, 5, 8}, {12, 12}).getWeekday(), weekday_t::FRIDAY);
    EXPECT_EQ(DateTime({2026, 5, 9}, {12, 12}).getWeekday(),
              weekday_t::SATURDAY);
}

TEST(TestDateTime, EqualDate) {
    EXPECT_TRUE(DateTime::equalDate(DateTime::now(), DateTime::now()));

    EXPECT_TRUE(DateTime::equalDate(DateTime({2026, 5, 5}, {12, 12}),
                                    DateTime({2026, 5, 5}, {0, 12})));

    EXPECT_FALSE(DateTime::equalDate(DateTime({2026, 6, 5}, {12, 12}),
                                     DateTime({2026, 5, 5}, {12, 12})));
}

TEST(TestDateTime, EqualTime) {
    EXPECT_TRUE(DateTime::equalTime(DateTime::now(), DateTime::now()));

    EXPECT_TRUE(DateTime::equalTime(DateTime({2026, 5, 5}, {12, 12}),
                                    DateTime({2025, 4, 4}, {12, 12})));

    EXPECT_FALSE(DateTime::equalTime(DateTime({2026, 5, 5}, {12, 12}),
                                     DateTime({2026, 5, 5}, {13, 12})));
}

TEST(TestDateTime, ComparisonOperators) {
    EXPECT_TRUE(DateTime::now() == DateTime::now());
    EXPECT_FALSE(DateTime::now() != DateTime::now());

    EXPECT_TRUE(DateTime::now() >= DateTime::now());
    EXPECT_TRUE(DateTime::now() <= DateTime::now());

    EXPECT_FALSE(DateTime::now() > DateTime::now());
    EXPECT_FALSE(DateTime::now() > DateTime::now());

    EXPECT_TRUE(DateTime({2024, 5, 5}) > DateTime({2023, 5, 5}));
    EXPECT_TRUE(DateTime({2024, 5, 5}) > DateTime({2024, 4, 5}));
    EXPECT_TRUE(DateTime({2024, 5, 5}) > DateTime({2024, 5, 4}));

    EXPECT_TRUE(DateTime({2024, 5, 5}, {12, 12}) ==
                DateTime({2024, 5, 5}, {12, 12}));

    EXPECT_TRUE(DateTime({2024, 5, 5}, {12, 12}) >
                DateTime({2024, 5, 5}, {11, 12}));

    EXPECT_TRUE(DateTime({2024, 5, 5}, {12, 12}) >
                DateTime({2024, 5, 5}, {12, 11}));

    EXPECT_TRUE(DateTime({2024, 5, 5}, {12, 12}) >
                DateTime({2024, 5, 5}, {0, 0}));
}

TEST(TestDateTime, ISO8601ValidInput) {
    auto datetime{DateTime::fromISO8601String("2024-12-01T23:40")};

    ASSERT_TRUE(datetime);
    EXPECT_EQ(datetime->getDate(), Date({2024, 12, 1}));
    EXPECT_EQ(datetime->getTime(), Time({23, 40}));

    auto leapYear{DateTime::fromISO8601String("2024-02-29T12:00")};

    ASSERT_TRUE(leapYear);
    EXPECT_EQ(leapYear->getDate(), Date({2024, 2, 29}));
    EXPECT_EQ(leapYear->getTime(), Time({12, 0}));
}

TEST(TestDateTime, ISO8601InvalidFormat) {
    EXPECT_FALSE(DateTime::fromISO8601String("hello"));

    /* missing hours */
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T:58"));

    /* missing minutes */
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T23"));

    /* missing time */
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T"));

    /* missing time separator */
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-0123:40"));

    /* missing date separator */
    EXPECT_FALSE(DateTime::fromISO8601String("202412-01T23:40"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-1201T23:40"));
    EXPECT_FALSE(DateTime::fromISO8601String("20241201T23:40"));
}

TEST(TestDateTime, ISO8601InvalidDate) {
    /* not a leap year */
    EXPECT_FALSE(DateTime::fromISO8601String("2023-02-29T00:00"));

    /* invalid day */
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-00T00:00"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-0T00:00"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-1T00:00"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-32T00:00"));

    /* invalid month */
    EXPECT_FALSE(DateTime::fromISO8601String("2024-00-01T00:00"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-0-01T00:00"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-13-01T00:00"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-1-01T00:00"));
}

TEST(TestDateTime, ISO8601InvalidTime) {
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T1:1"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T0:0"));

    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T24:00"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T12:60"));

    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T99:99"));
}

TEST(TestDateTime, ISO8601SecondsAreIgnored) {
    auto dt1{DateTime::fromISO8601String("2024-12-01T12:30:59")};
    auto dt2{DateTime::fromISO8601String("2024-12-01T12:30:00")};

    ASSERT_TRUE(dt1);
    ASSERT_TRUE(dt2);

    EXPECT_EQ(dt1->getTime(), dt2->getTime());
}

TEST(TestDateTime, AddSimpleDateIntervals) {
    auto datetime{DateTime({2026, 5, 5}, {19, 59})};

    /* add days */
    EXPECT_EQ(datetime + Interval::days(1), DateTime({2026, 5, 6}, {19, 59}));
    EXPECT_EQ(datetime + Interval::days(2), DateTime({2026, 5, 7}, {19, 59}));
    EXPECT_EQ(datetime + Interval::days(10000),
              DateTime({2053, 9, 20}, {19, 59}));

    /* add weeks */
    EXPECT_EQ(datetime + Interval::weeks(1), DateTime({2026, 5, 12}, {19, 59}));
    EXPECT_EQ(datetime + Interval::weeks(2), DateTime({2026, 5, 19}, {19, 59}));
    EXPECT_EQ(datetime + Interval::weeks(10000),
              DateTime({2217, 12, 30}, {19, 59}));

    /* week is a multiple of day */
    EXPECT_EQ(datetime + Interval::days(7), datetime + Interval::weeks(1));
    EXPECT_EQ(datetime + Interval::days(14), datetime + Interval::weeks(2));
    EXPECT_EQ(datetime + Interval::days(49), datetime + Interval::weeks(7));

    /* add months */
    EXPECT_EQ(datetime + Interval::months(1), DateTime({2026, 6, 5}, {19, 59}));
    EXPECT_EQ(datetime + Interval::months(2), DateTime({2026, 7, 5}, {19, 59}));
    EXPECT_EQ(datetime + Interval::months(1000),
              DateTime({2109, 9, 5}, {19, 59}));

    /* add years */
    EXPECT_EQ(datetime + Interval::years(1), DateTime({2027, 5, 5}, {19, 59}));
    EXPECT_EQ(datetime + Interval::years(2), DateTime({2028, 5, 5}, {19, 59}));
    EXPECT_EQ(datetime + Interval::years(1000),
              DateTime({3026, 5, 5}, {19, 59}));

    /* year is a multiple of month */
    EXPECT_EQ(datetime + Interval::months(12), datetime + Interval::years(1));
    EXPECT_EQ(datetime + Interval::months(24), datetime + Interval::years(2));
    EXPECT_EQ(datetime + Interval::months(120), datetime + Interval::years(10));
}

TEST(TestDateTime, AddMixedDateIntervals) {
    auto datetime{DateTime({2026, 5, 5}, {19, 59})};

    EXPECT_EQ(datetime +
                  Interval({.years = 1, .months = 1, .weeks = 1, .days = 1}),
              DateTime({2027, 6, 13}, {19, 59}));

    EXPECT_EQ(datetime +
                  Interval({.years = 2, .months = 2, .weeks = 2, .days = 2}),
              DateTime({2028, 7, 21}, {19, 59}));

    EXPECT_EQ(datetime +
                  Interval({.years = 1, .months = 2, .weeks = 3, .days = 4}),
              DateTime({2027, 7, 30}, {19, 59}));
}

TEST(TestDateTime, AddSimpleTimeIntervals) {
    auto datetime{DateTime({2026, 5, 5}, {19, 59})};

    /* add minutes */
    EXPECT_EQ(datetime + Interval::minutes(1),
              DateTime({2026, 5, 5}, {20, 00}));
    EXPECT_EQ(datetime + Interval::minutes(2),
              DateTime({2026, 5, 5}, {20, 01}));

    /* add hours */
    EXPECT_EQ(datetime + Interval::hours(1), DateTime({2026, 5, 5}, {20, 59}));
    EXPECT_EQ(datetime + Interval::hours(2), DateTime({2026, 5, 5}, {21, 59}));

    /* hour is a multiple of minute */
    EXPECT_EQ(datetime + Interval::minutes(60), datetime + Interval::hours(1));
    EXPECT_EQ(datetime + Interval::minutes(120), datetime + Interval::hours(2));
    EXPECT_EQ(datetime + Interval::minutes(600),
              datetime + Interval::hours(10));

    /* day is a multiple of hour */
    EXPECT_EQ(datetime + Interval::hours(24), datetime + Interval::days(1));
    EXPECT_EQ(datetime + Interval::hours(48), datetime + Interval::days(2));
    EXPECT_EQ(datetime + Interval::hours(240), datetime + Interval::days(10));
}

TEST(TestDateTime, AddMixedTimeIntervals) {
    auto datetime{DateTime({2026, 5, 5}, {19, 59})};

    EXPECT_EQ(datetime + Interval({.hours = 1, .minutes = 1}),
              DateTime({2026, 5, 5}, {21, 00}));

    EXPECT_EQ(datetime + Interval({.hours = 2, .minutes = 2}),
              DateTime({2026, 5, 5}, {22, 01}));

    EXPECT_EQ(datetime + Interval({.hours = 1, .minutes = 2}),
              DateTime({2026, 5, 5}, {21, 01}));
}

TEST(TestDateTime, AddDateTimeIntervals) {
    auto datetime{DateTime({2026, 5, 5}, {19, 59})};

    EXPECT_EQ(datetime + Interval({.days = 1, .minutes = 1}),
              DateTime({2026, 5, 6}, {20, 00}));

    EXPECT_EQ(datetime + Interval({.years = 1,
                                   .months = 1,
                                   .weeks = 1,
                                   .days = 1,
                                   .hours = 1,
                                   .minutes = 1}),
              DateTime({2027, 6, 13}, {21, 00}));

    EXPECT_EQ(datetime + Interval({.years = 2,
                                   .months = 2,
                                   .weeks = 2,
                                   .days = 2,
                                   .hours = 2,
                                   .minutes = 2}),
              DateTime({2028, 7, 21}, {22, 01}));
}

TEST(TestDateTime, AddIntervalOnHourBorder) {
    auto datetime{DateTime({2026, 5, 5}, {19, 59})};

    EXPECT_EQ(datetime + Interval::minutes(1),
              DateTime({2026, 5, 5}, {20, 00}));
}

TEST(TestDateTime, AddIntervalOnDayBorder) {
    auto datetime{DateTime({2026, 5, 5}, {23, 59})};
    EXPECT_EQ(datetime + Interval::minutes(1),
              DateTime({2026, 5, 6}, {00, 00}));

    datetime = DateTime({2026, 5, 5}, {22, 59});
    EXPECT_EQ(datetime + Interval({.hours = 1, .minutes = 1}),
              DateTime({2026, 5, 6}, {00, 00}));
}

TEST(TestDateTime, AddIntervalOnMonthBorder) {
    auto datetime{DateTime({2026, 5, 31}, {23, 59})};
    EXPECT_EQ(datetime + Interval::minutes(1),
              DateTime({2026, 6, 1}, {00, 00}));

    datetime = DateTime({2026, 5, 30}, {22, 59});
    EXPECT_EQ(datetime + Interval({.days = 1, .hours = 1, .minutes = 1}),
              DateTime({2026, 6, 1}, {00, 00}));
}

TEST(TestDateTime, AddIntervalOnYearBorder) {
    auto datetime{DateTime({2026, 12, 31}, {23, 59})};
    EXPECT_EQ(datetime + Interval::minutes(1),
              DateTime({2027, 1, 1}, {00, 00}));

    datetime = DateTime({2025, 11, 30}, {22, 59});
    EXPECT_EQ(
        datetime +
            Interval(
                {.years = 1, .months = 1, .days = 1, .hours = 1, .minutes = 1}),
        DateTime({2027, 1, 1}, {00, 00}));
}

TEST(TestDateTime, AddIntervalOnLeapYearFebruaryBorder) {
    auto datetime{DateTime({2024, 2, 28}, {23, 59})};
    EXPECT_EQ(datetime + Interval::minutes(1),
              DateTime({2024, 2, 29}, {00, 00}));

    datetime = DateTime({2023, 1, 27}, {22, 59});
    EXPECT_EQ(
        datetime +
            Interval(
                {.years = 1, .months = 1, .days = 1, .hours = 1, .minutes = 1}),
        DateTime({2024, 2, 29}, {0, 0}));
}

TEST(TestDateTime, DiffBetweenDateTimes) {
    auto dt1{DateTime({2026, 5, 5}, {19, 59})};
    auto dt2{DateTime({2025, 4, 4}, {18, 58})};

    EXPECT_EQ(DateTime::diff(dt1, dt2), Interval::minutes(570301));
    EXPECT_EQ(DateTime::diff(dt1, dt2), DateTime::diff(dt2, dt1));

    dt1 = DateTime::now();
    dt2 = DateTime::now();
    EXPECT_EQ(DateTime::diff(dt1, dt2), Interval());
}

TEST(TestDateTime, DiffBetweenDateTimesOnLeapYear) {
    EXPECT_EQ(DateTime::diff(DateTime({2024, 2, 28}), DateTime({2024, 3, 1})),
              Interval::minutes(2880));

    EXPECT_EQ(DateTime::diff(DateTime({2023, 2, 28}, {22, 59}),
                             DateTime({2024, 3, 1}, {0, 0})),
              Interval::minutes(527101));
}

TEST(TestDateTime, DaysDiffBetweenDateTimes) {
    auto dt1{DateTime({2026, 5, 5}, {19, 59})};
    auto dt2{DateTime({2025, 4, 4}, {18, 58})};

    EXPECT_EQ(DateTime::daysDiff(dt1, dt2), Interval::days(396));
}

TEST(TestDateTime, DaysDiffBetweenDateTimesOnLeapYear) {
    EXPECT_EQ(
        DateTime::daysDiff(DateTime({2024, 2, 28}), DateTime({2024, 3, 1})),
        Interval::days(2));

    EXPECT_EQ(DateTime::daysDiff(DateTime({2023, 2, 28}, {22, 59}),
                                 DateTime({2024, 3, 1}, {0, 0})),
              Interval::days(366));
}

TEST(TestDateTime, DiffAndIntervalCancelEachOtherOut) {
    auto dt1{DateTime({2026, 5, 5}, {19, 59})};
    auto dt2{DateTime({2025, 4, 4}, {18, 58})};

    auto diff(DateTime::diff(dt1, dt2));
    EXPECT_EQ(dt2 + diff, dt1);

    /* on leap year */
    dt1 = DateTime({2025, 5, 5}, {19, 59});
    dt2 = DateTime({2023, 4, 4}, {18, 58});

    diff = DateTime::diff(dt1, dt2);
    EXPECT_EQ(dt2 + diff, dt1);

    dt1 = DateTime({2025, 5, 5}, {19, 59});
    dt2 = DateTime({1023, 2, 2}, {10, 58});

    diff = DateTime::diff(dt1, dt2);
    EXPECT_EQ(dt2 + diff, dt1);
}
} // namespace test::mods
