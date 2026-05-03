#include <duration.hpp>
#include <ymd.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>

namespace test::mods {
using hbt::mods::Duration;
using hbt::mods::YMD;

using std::chrono::year_month_day, std::chrono::year, std::chrono::month,
    std::chrono::day;

TEST(YMDTest, ConstructsValidYMD) {
    EXPECT_NO_THROW(YMD(2026, 5, 2));

    EXPECT_NO_THROW(YMD(100, 5, 2));

    EXPECT_NO_THROW(YMD(0, 1, 1));

    EXPECT_NO_THROW(YMD(-1, 1, 1));
}

TEST(YMDTest, ValidYMDIsOK) {
    EXPECT_TRUE(YMD(2026, 5, 2).ok());

    EXPECT_TRUE(YMD(100, 5, 2).ok());

    EXPECT_TRUE(YMD(0, 1, 1).ok());

    EXPECT_TRUE(YMD(-1, 1, 1).ok());
}

TEST(YMDTest, ThrowsOnInvalidParameters) {
    EXPECT_THROW(YMD(2026, 2, 29), std::invalid_argument); // not a leap year

    EXPECT_THROW(YMD(2026, 13, 1), std::invalid_argument); // invalid month

    EXPECT_THROW(YMD(2026, 1, 32), std::invalid_argument); // january has 31 day

    EXPECT_THROW(YMD(2026, 13, 32),
                 std::invalid_argument); // invalid month & day

    EXPECT_THROW(YMD(2026, 0, 0),
                 std::invalid_argument); // invalid month & day

    EXPECT_THROW(YMD(2026, -1, -1),
                 std::invalid_argument); // invalid month & day
}

TEST(YMDTest, Getters) {
    auto ymd{YMD(2026, 5, 2)};

    EXPECT_EQ(ymd.getYear(), 2026);
    EXPECT_EQ(ymd.getMonth(), 5);
    EXPECT_EQ(ymd.getDay(), 2);

    EXPECT_EQ(ymd.getWeekday(), YMD::weekday_t::SATURDAY);
    EXPECT_EQ(YMD(2026, 5, 3).getWeekday(), YMD::weekday_t::SUNDAY);
}

TEST(YMDTest, ToFromChrono) {
    auto chrono{year_month_day(year(2026), month(5), day(2))};
    assert(chrono.ok());

    auto from{YMD::fromChrono(chrono)};
    ASSERT_TRUE(from);

    auto to{from.value().toChrono()};
    ASSERT_TRUE(to.ok());

    EXPECT_EQ(chrono, to);
}

TEST(YMDTest, TodayIsOK) {
    ASSERT_NO_THROW(YMD::today());

    ASSERT_TRUE(YMD::today().ok());

    auto to{YMD::today().toChrono()};
    ASSERT_TRUE(to.ok());

    EXPECT_EQ(YMD::fromChrono(to), YMD::today());
}

TEST(YMDTest, ComparisonOperators) {
    auto ymd1{YMD(2026, 4, 26)};
    auto ymd2{YMD(2026, 4, 27)};

    EXPECT_TRUE(ymd1 < ymd2);
    EXPECT_TRUE(ymd1 <= ymd2);

    EXPECT_TRUE(ymd2 > ymd1);
    EXPECT_TRUE(ymd2 >= ymd1);

    EXPECT_FALSE(ymd1 == ymd2);
    EXPECT_TRUE(ymd1 != ymd2);

    ymd1 = YMD(2026, 4, 26);
    ymd2 = YMD(2026, 4, 27);

    EXPECT_TRUE(ymd1 < ymd2);
    EXPECT_TRUE(ymd1 <= ymd2);

    EXPECT_TRUE(ymd2 > ymd1);
    EXPECT_TRUE(ymd2 >= ymd1);

    EXPECT_FALSE(ymd1 == ymd2);
    EXPECT_TRUE(ymd1 != ymd2);
}

} // namespace test::mods
