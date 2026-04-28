#include <interval.hpp>
#include <time.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test::mods {
using hbt::mods::Time;
using value_t = hbt::mods::Time::value_t;
using hours_t = hbt::mods::Time::hours_t;
using minutes_t = hbt::mods::Time::minutes_t;

TEST(TimeTest, ThrowsOnInvalidValue) {
    EXPECT_THROW(Time(value_t{-1}), std::invalid_argument);
}

TEST(TimeTest, ThrowsOnInvalidHourAndMinute) {
    EXPECT_THROW(Time(hours_t(-1), minutes_t(1)), std::invalid_argument);

    EXPECT_THROW(Time(hours_t(1), minutes_t(-1)), std::invalid_argument);

    EXPECT_THROW(Time(hours_t(-1), minutes_t(-1)), std::invalid_argument);

    EXPECT_THROW(Time(hours_t(24), minutes_t(1)), std::invalid_argument);

    EXPECT_THROW(Time(hours_t(1), minutes_t(60)), std::invalid_argument);

    EXPECT_THROW(Time(hours_t(24), minutes_t(60)), std::invalid_argument);
}

TEST(TimeTest, Getters) {
    auto t1{Time{value_t{0}}};
    EXPECT_EQ(t1.getValue(), value_t{0});

    auto t2{Time{hours_t{1}, minutes_t{1}}};
    EXPECT_EQ(t1.getHour(), value_t{1});
    EXPECT_EQ(t1.getMinute(), value_t{1});
    EXPECT_EQ(t1.getValue(), value_t{Time::timeInHour.count() * t1.getHour() +
                                     t1.getMinute()});
}

TEST(TimeTest, ComparisonOperators) {
    auto t1{Time{value_t{0}}};
    auto t2{Time{value_t{1}}};

    EXPECT_TRUE(t1 < t2);
    EXPECT_TRUE(t1 <= t2);

    EXPECT_TRUE(t2 > t1);
    EXPECT_TRUE(t2 >= t1);

    EXPECT_FALSE(t1 == t2);
    EXPECT_TRUE(t1 != t2);

    auto t3{Time{hours_t{1}, minutes_t{0}}};
    auto t4{Time{hours_t{0}, minutes_t{0}}};

    EXPECT_TRUE(t3 < t4);
    EXPECT_TRUE(t3 <= t4);

    EXPECT_TRUE(t4 > t3);
    EXPECT_TRUE(t4 >= t3);

    EXPECT_FALSE(t3 == t4);
    EXPECT_TRUE(t3 != t4);
}

TEST(TimeTest, OperatorPlus) {
    auto t{Time{hours_t{0}, minutes_t{0}}};

    auto res1 = t + hbt::mods::Interval{
                        hbt::mods::Duration::Units{.hours{1}, .minutes{1}}};
    EXPECT_EQ(res1.first.getHour(), value_t{1});
    EXPECT_EQ(res1.first.getMinute(), value_t{1});
    EXPECT_EQ(res1.second, false);

    auto res2 = t + hbt::mods::Interval{
                        hbt::mods::Duration::Units{.hours{24}, .minutes{0}}};
    EXPECT_EQ(res2.first.getHour(), value_t{0});
    EXPECT_EQ(res2.first.getMinute(), value_t{0});
    EXPECT_EQ(res2.second, true);

    auto res3 = t + hbt::mods::Interval{hbt::mods::Duration::Units{
                        .hours{0}, .minutes{Time::timeInDay.count()}}};
    EXPECT_EQ(res3.first.getHour(), value_t{0});
    EXPECT_EQ(res3.first.getMinute(), value_t{0});
    EXPECT_EQ(res3.second, true);

    auto res4 = t + hbt::mods::Interval{
                        hbt::mods::Duration::Units{.hours{24}, .minutes{1}}};
    EXPECT_EQ(res4.first.getHour(), value_t{0});
    EXPECT_EQ(res4.first.getMinute(), value_t{1});
    EXPECT_EQ(res4.second, true);
}

TEST(TimeTest, GetDiff) {
    auto t1{Time{hours_t{0}, minutes_t{0}}};
    auto t2{Time{hours_t{0}, minutes_t{0}}};
    EXPECT_EQ(Time::getDiff(t1, t2), hbt::mods::Duration{});
    EXPECT_EQ(Time::getDiff(t2, t1), hbt::mods::Duration{});

    auto t3{Time{hours_t{1}, minutes_t{1}}};
    auto t4{Time{hours_t{0}, minutes_t{0}}};
    EXPECT_EQ(Time::getDiff(t3, t4),
              hbt::mods::Duration(
                  hbt::mods::Duration::Units{.hours{1}, .minutes{1}}));
    EXPECT_EQ(Time::getDiff(t4, t3),
              hbt::mods::Duration(
                  hbt::mods::Duration::Units{.hours{1}, .minutes{1}}));
}
} // namespace test::mods
