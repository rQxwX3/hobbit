#include <gtest/gtest.h>

#include <date.hpp>
#include <date_calculator.hpp>
#include <duration.hpp>

namespace test::mods::util {
using hbt::mods::Date;
using hbt::mods::Duration;
using hbt::mods::util::DateCalculator;

TEST(DateCalculatorTest, DaysBetweenBasics) {
    EXPECT_EQ(DateCalculator::daysBetween(Date::today(), Date::today()), 0);

    EXPECT_EQ(DateCalculator::daysBetween(Date(2026, 5, 1), Date(2026, 5, 2)),
              1);
    EXPECT_EQ(DateCalculator::daysBetween(Date(2026, 5, 2), Date(2026, 5, 1)),
              1);
}

TEST(DateCalculatorTest, DaysBetweenMonthBoundaries) {
    EXPECT_EQ(DateCalculator::daysBetween(Date(2026, 1, 31), Date(2026, 2, 1)),
              1);

    EXPECT_EQ(DateCalculator::daysBetween(Date(2026, 4, 30), Date(2026, 5, 1)),
              1);
}

TEST(DateCalculatorTest, DaysBetweenYearBoundaries) {
    EXPECT_EQ(DateCalculator::daysBetween(Date(2025, 12, 31), Date(2026, 1, 1)),
              1);

    EXPECT_EQ(DateCalculator::daysBetween(Date(2020, 1, 1), Date(2021, 1, 1)),
              366); // leap year included
}

TEST(DateCalculatorTest, DaysBetweenLeapYear) {
    EXPECT_EQ(DateCalculator::daysBetween(Date(2024, 2, 28), Date(2024, 2, 29)),
              1);

    EXPECT_EQ(DateCalculator::daysBetween(Date(2024, 2, 29), Date(2025, 2, 28)),
              365);
}

TEST(DateCalculatorTest, DaysBetweenAdditivity) {
    Date a(2020, 1, 1);
    Date b(2023, 6, 15);
    Date c(2026, 5, 3);

    EXPECT_EQ(DateCalculator::daysBetween(a, c),
              DateCalculator::daysBetween(a, b) +
                  DateCalculator::daysBetween(b, c));
}

TEST(DateCalculatorTest, DaysBetweenLongRange) {
    EXPECT_EQ(DateCalculator::daysBetween(Date(2026, 5, 3), Date(2020, 1, 1)),
              2314);

    EXPECT_EQ(DateCalculator::daysBetween(Date(2026, 5, 3), Date(2000, 1, 1)),
              9619);
}

// TEST(DateCalculatorTest, DifferenceCalendar) {
//     EXPECT_EQ(DateCalculator::differenceCalendar(Date::today(),
//     Date::today()),
//               Duration{});
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 5, 2), Date(2026, 5,
//         1)), Duration::days(1));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 5, 1), Date(2026, 5,
//         2)), Duration::days(1));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 5, 2), Date(2026, 4,
//         2)), Duration::months(1));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 4, 2), Date(2026, 5,
//         2)), Duration::months(1));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 5, 2), Date(2025, 5,
//         2)), Duration::years(1));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 5, 2), Date(2026, 5,
//         2)), Duration::years(1));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 5, 2), Date(2026, 4,
//         1)), Duration({.months = 1, .days = 1}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 4, 1), Date(2026, 5,
//         2)), Duration({.months = 1, .days = 1}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 4, 1), Date(2026, 5,
//         2)), Duration({.years = 1, .months = 1, .days = 1}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 5, 2), Date(2025, 4,
//         1)), Duration({.years = 1, .months = 1, .days = 1}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 4, 1), Date(2026, 3,
//         1)), Duration({.months = 11}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 3, 1), Date(2025, 4,
//         1)), Duration({.months = 11}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 4, 2), Date(2026, 3,
//         1)), Duration({.months = 10, .days = 27}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 3, 1), Date(2025, 4,
//         2)), Duration({.months = 10, .days = 27}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 2, 10), Date(2025, 3,
//         1)), Duration({.days = 19}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 3, 1), Date(2025, 2,
//         10)), Duration({.days = 19}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2024, 1, 1), Date(2025, 1,
//         1)), Duration({.years = 1}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 1, 1), Date(2024, 1,
//         1)), Duration({.years = 1}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2024, 2, 28), Date(2024, 3,
//         1)), Duration({.days = 2}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2024, 3, 1), Date(2024, 2,
//         28)), Duration({.days = 2}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 2, 28), Date(2023, 3,
//         1)), Duration({.days = 1}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 3, 1), Date(2023, 2,
//         28)), Duration({.days = 1}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 1, 31), Date(2023, 2,
//         28)), Duration({.days = 28}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 2, 28), Date(2023, 1,
//         31)), Duration({.days = 28}));
//
//     /* differenceHuman is expected to produce diff results on the tests below
//     */ EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 1, 31), Date(2023, 3,
//         1)), Duration({.days = 29}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 3, 1), Date(2023, 1,
//         31)), Duration({.days = 29}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 1, 30), Date(2023, 3,
//         1)), Duration({.days = 30}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 3, 1), Date(2023, 1,
//         30)), Duration({.days = 30}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 3, 31), Date(2023, 5,
//         1)), Duration({.days = 29}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 5, 1), Date(2023, 3,
//         31)), Duration({.days = 29}));
//     /* differenceHuman is expected to produce diff results on the tests above
//     */
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 12, 31), Date(2026, 1,
//         1)), Duration({.days = 1}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 1, 1), Date(2025, 12,
//         31)), Duration({.days = 1}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 12, 1), Date(2026, 1,
//         1)), Duration({.months = 1}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 1, 1), Date(2025, 12,
//         1)), Duration({.months = 1}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2024, 2, 29), Date(2028, 2,
//         29)), Duration({.years = 4}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2028, 2, 29), Date(2024, 2,
//         29)), Duration({.years = 4}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2024, 2, 29), Date(2025, 2,
//         28)), Duration({.months = 11, .days = 30}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2025, 2, 28), Date(2024, 2,
//         29)), Duration({.months = 11, .days = 30}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 1, 31), Date(2023, 3,
//         31)), Duration({.months = 2}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 3, 31), Date(2023, 1,
//         31)), Duration({.months = 2}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 1, 31), Date(2023, 4,
//         30)), Duration({.months = 2, .days = 30}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 4, 30), Date(2023, 1,
//         31)), Duration({.months = 2, .days = 30}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2024, 1, 31), Date(2024, 3,
//         1)), Duration({.days = 30}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2024, 3, 1), Date(2024, 1,
//         31)), Duration({.days = 30}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 1, 30), Date(2023, 2,
//         1)), Duration({.days = 2}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 2, 1), Date(2023, 1,
//         30)), Duration({.days = 2}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2000, 1, 1), Date(2026, 5,
//         2)), Duration({.years = 26, .months = 4, .days = 1}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2026, 5, 2), Date(2000, 1,
//         1)), Duration({.years = 26, .months = 4, .days = 1}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 4, 30), Date(2023, 5,
//         31)), Duration({.months = 1, .days = 1}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 5, 31), Date(2023, 4,
//         30)), Duration({.months = 1, .days = 1}));
//
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 1, 15), Date(2023, 2,
//         14)), Duration({.days = 30}));
//     EXPECT_EQ(
//         DateCalculator::differenceCalendar(Date(2023, 2, 14), Date(2023, 1,
//         15)), Duration({.days = 30}));
// }
} // namespace test::mods::util
