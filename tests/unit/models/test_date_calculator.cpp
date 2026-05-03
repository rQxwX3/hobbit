#include <gtest/gtest.h>

#include <date.hpp>
#include <date_calculator.hpp>
#include <duration.hpp>

namespace test::mods::util {
using hbt::mods::Date;
using hbt::mods::Duration;
using hbt::mods::util::DateCalculator;

TEST(DateCalculatorTest, FindsDifferenceCorrectly) {
    EXPECT_EQ(DateCalculator::difference(Date::today(), Date::today()),
              Duration{});

    EXPECT_EQ(DateCalculator::difference(Date(2026, 5, 2), Date(2026, 5, 1)),
              Duration::days(1));
    EXPECT_EQ(DateCalculator::difference(Date(2026, 5, 1), Date(2026, 5, 2)),
              Duration::days(1));

    EXPECT_EQ(DateCalculator::difference(Date(2026, 5, 2), Date(2026, 4, 2)),
              Duration::months(1));
    EXPECT_EQ(DateCalculator::difference(Date(2026, 4, 2), Date(2026, 5, 2)),
              Duration::months(1));

    EXPECT_EQ(DateCalculator::difference(Date(2026, 5, 2), Date(2025, 5, 2)),
              Duration::years(1));
    EXPECT_EQ(DateCalculator::difference(Date(2025, 5, 2), Date(2026, 5, 2)),
              Duration::years(1));

    EXPECT_EQ(DateCalculator::difference(Date(2026, 5, 2), Date(2026, 4, 1)),
              Duration({.months = 1, .days = 1}));
    EXPECT_EQ(DateCalculator::difference(Date(2026, 4, 1), Date(2026, 5, 2)),
              Duration({.months = 1, .days = 1}));

    EXPECT_EQ(DateCalculator::difference(Date(2025, 4, 1), Date(2026, 5, 2)),
              Duration({.years = 1, .months = 1, .days = 1}));
    EXPECT_EQ(DateCalculator::difference(Date(2026, 5, 2), Date(2025, 4, 1)),
              Duration({.years = 1, .months = 1, .days = 1}));

    EXPECT_EQ(DateCalculator::difference(Date(2025, 4, 1), Date(2026, 3, 1)),
              Duration({.months = 11}));
    EXPECT_EQ(DateCalculator::difference(Date(2026, 3, 1), Date(2025, 4, 1)),
              Duration({.months = 11}));

    EXPECT_EQ(DateCalculator::difference(Date(2025, 4, 2), Date(2026, 3, 1)),
              Duration({.months = 10, .days = 27}));
    EXPECT_EQ(DateCalculator::difference(Date(2026, 3, 1), Date(2025, 4, 2)),
              Duration({.months = 10, .days = 27}));

    EXPECT_EQ(DateCalculator::difference(Date(2025, 2, 10), Date(2025, 3, 1)),
              Duration({.days = 19}));
    EXPECT_EQ(DateCalculator::difference(Date(2025, 3, 1), Date(2025, 2, 10)),
              Duration({.days = 19}));

    EXPECT_EQ(DateCalculator::difference(Date(2024, 1, 1), Date(2025, 1, 1)),
              Duration({.years = 1}));
    EXPECT_EQ(DateCalculator::difference(Date(2025, 1, 1), Date(2024, 1, 1)),
              Duration({.years = 1}));

    EXPECT_EQ(DateCalculator::difference(Date(2024, 2, 28), Date(2024, 3, 1)),
              Duration({.days = 2}));
    EXPECT_EQ(DateCalculator::difference(Date(2024, 3, 1), Date(2024, 2, 28)),
              Duration({.days = 2}));

    EXPECT_EQ(DateCalculator::difference(Date(2023, 2, 28), Date(2023, 3, 1)),
              Duration({.days = 1}));
    EXPECT_EQ(DateCalculator::difference(Date(2023, 3, 1), Date(2023, 2, 28)),
              Duration({.days = 1}));

    EXPECT_EQ(DateCalculator::difference(Date(2023, 1, 31), Date(2023, 2, 28)),
              Duration({.days = 28}));
    EXPECT_EQ(DateCalculator::difference(Date(2023, 2, 28), Date(2023, 1, 31)),
              Duration({.days = 28}));

    EXPECT_EQ(DateCalculator::difference(Date(2023, 1, 31), Date(2023, 3, 1)),
              Duration({.months = 1, .days = 1}));

    EXPECT_EQ(DateCalculator::difference(Date(2023, 3, 1), Date(2023, 1, 31)),
              Duration({.months = 1, .days = 1}))
        << DateCalculator::difference(Date(2023, 3, 1), Date(2023, 1, 31))
               .toISO8601String()
        << '\n';

    EXPECT_EQ(DateCalculator::difference(Date(2025, 12, 31), Date(2026, 1, 1)),
              Duration({.days = 1}));
    EXPECT_EQ(DateCalculator::difference(Date(2026, 1, 1), Date(2025, 12, 31)),
              Duration({.days = 1}));

    EXPECT_EQ(DateCalculator::difference(Date(2025, 12, 1), Date(2026, 1, 1)),
              Duration({.months = 1}));
    EXPECT_EQ(DateCalculator::difference(Date(2026, 1, 1), Date(2025, 12, 1)),
              Duration({.months = 1}));

    EXPECT_EQ(DateCalculator::difference(Date(2024, 2, 29), Date(2028, 2, 29)),
              Duration({.years = 4}));
    EXPECT_EQ(DateCalculator::difference(Date(2028, 2, 29), Date(2024, 2, 29)),
              Duration({.years = 4}));

    EXPECT_EQ(DateCalculator::difference(Date(2024, 2, 29), Date(2025, 2, 28)),
              Duration({.months = 11, .days = 30}))
        << DateCalculator::difference(Date(2024, 2, 29), Date(2025, 2, 28))
               .toISO8601String()
        << '\n';
    EXPECT_EQ(DateCalculator::difference(Date(2025, 2, 28), Date(2024, 2, 29)),
              Duration({.months = 11, .days = 30}));
}
} // namespace test::mods::util
