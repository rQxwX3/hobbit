#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>

namespace test::mods {
using hbt::mods::Date;
using std::chrono::day;
using std::chrono::month;
using std::chrono::year;
using std::chrono::year_month_day;

using hbt::mods::DateTime;

using hbt::mods::Time;
using hours_t = hbt::mods::Time::hours_t;
using minutes_t = hbt::mods::Time::minutes_t;

using hbt::mods::Interval;

TEST(TestDateTime, Getters) {
    auto dt{DateTime{Date::today(), Time::now()}};

    EXPECT_EQ(dt.getDate(), Date::today());
    EXPECT_EQ(dt.getTime(), Time::now());
}

TEST(TestDateTime, EqualDate) {
    auto dt1{DateTime{Date::today(), Time::now()}};
    auto dt2{DateTime{Date::today(), Time::now()}};
    EXPECT_TRUE(DateTime::equalDate(dt1, dt2));
    EXPECT_TRUE(DateTime::equalDate(dt2, dt1));

    dt1 = DateTime{Date::today(), Time{hours_t{1}, minutes_t{1}}};
    dt2 = DateTime{Date::today(), Time{hours_t{0}, minutes_t{0}}};
    EXPECT_TRUE(DateTime::equalDate(dt1, dt2));
    EXPECT_TRUE(DateTime::equalDate(dt2, dt1));

    dt1 = DateTime{Date{year_month_day{year(2026), month(4), day(27)}},
                   Time::now()};
    dt2 = DateTime{Date{year_month_day{year(2026), month(4), day(28)}},
                   Time::now()};
    EXPECT_FALSE(DateTime::equalDate(dt1, dt2));
    EXPECT_FALSE(DateTime::equalDate(dt2, dt1));
}

TEST(TestDateTime, EqualTime) {
    auto dt1{DateTime{Date::today(), Time::now()}};
    auto dt2{DateTime{Date::today(), Time::now()}};
    EXPECT_TRUE(DateTime::equalTime(dt1, dt2));
    EXPECT_TRUE(DateTime::equalTime(dt2, dt1));

    auto dt3{DateTime{Date{year_month_day{year(2026), month(4), day(27)}},
                      Time::now()}};
    auto dt4{DateTime{Date{year_month_day(year(2025), month(5), day(28))},
                      Time::now()}};
    EXPECT_TRUE(DateTime::equalTime(dt3, dt4));
    EXPECT_TRUE(DateTime::equalTime(dt4, dt3));
}

TEST(TestDateTime, ToFromISO8601) {
    auto original{DateTime{year_month_day(year{2026}, month{4}, day{27}),
                           Time{hours_t{12}, minutes_t{12}}}};

    auto ymdString{original.toISO8601String()};
    EXPECT_EQ(ymdString, "2026-04-27T12:12");

    auto restored{DateTime::fromISO8601String(ymdString)};
    ASSERT_TRUE(restored);
    EXPECT_EQ(restored, original);
}

TEST(TestDateTime, FromValidISO8601) {
    auto dt1{DateTime::fromISO8601String("2024-12-01T12:30")};
    EXPECT_TRUE(dt1);
    EXPECT_EQ(dt1->getDate(),
              Date{year_month_day(year(2024), month(12), day(1))});
    EXPECT_EQ(dt1->getTime(), Time(hours_t{12}, minutes_t{30}));

    auto dt2{DateTime::fromISO8601String("1000/01/30T00:30:12")};
    EXPECT_TRUE(dt2);
    EXPECT_EQ(dt2->getDate(),
              Date{year_month_day(year(1000), month(1), day(30))});
    EXPECT_EQ(dt2->getTime(), Time(hours_t{0}, minutes_t{30}));

    auto dt3{DateTime::fromISO8601String("0012.10.01T23:50:00")};
    EXPECT_TRUE(dt3);
    EXPECT_EQ(dt3->getDate(),
              Date{year_month_day(year(12), month(10), day(1))});
    EXPECT_EQ(dt3->getTime(), Time(hours_t{23}, minutes_t{50}));

    auto dt4{DateTime::fromISO8601String("2024-12-01T02:10")};
    EXPECT_TRUE(dt4);
    EXPECT_EQ(dt4->getDate(),
              Date{year_month_day(year(2024), month(12), day(1))});
    EXPECT_EQ(dt4->getTime(), Time(hours_t{2}, minutes_t{10}));
}

TEST(DateTimeTest, FromInvalidISO8601) {
    EXPECT_FALSE(DateTime::fromISO8601String(""));
    EXPECT_FALSE(DateTime::fromISO8601String("hello"));

    EXPECT_FALSE(DateTime::fromISO8601String("2026"));

    EXPECT_FALSE(DateTime::fromISO8601String("20261231"));

    EXPECT_FALSE(DateTime::fromISO8601String("2026-12"));

    // missing time section
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T12"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T12:"));

    // invalid hour
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T-1:00"));

    // invalid minute
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T22:-1"));

    // invalid month
    EXPECT_FALSE(DateTime::fromISO8601String("2026-13-31"));

    // invalid day
    EXPECT_FALSE(DateTime::fromISO8601String("2026-12-38"));

    // non-leap year Febrary 29th
    EXPECT_FALSE(DateTime::fromISO8601String("2026-02-29"));

    // single digit sections
    EXPECT_FALSE(DateTime::fromISO8601String("2026-2-29"));
    EXPECT_FALSE(DateTime::fromISO8601String("2026-02-1"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T12:1"));
    EXPECT_FALSE(DateTime::fromISO8601String("2024-12-01T2:10"));
}

TEST(TestDateTime, ComparisonOperators) {
    auto dt1{DateTime{Date::today(), Time::now()}};
    auto dt2{DateTime{Date(year{2024}, month{1}, day{1})}};

    EXPECT_TRUE(dt1 > dt2);
    EXPECT_TRUE(dt1 >= dt2);

    EXPECT_TRUE(dt2 < dt1);
    EXPECT_TRUE(dt2 <= dt1);

    EXPECT_FALSE(dt1 == dt2);
    EXPECT_TRUE(dt1 != dt2);

    EXPECT_FALSE(dt2 == dt1);
    EXPECT_TRUE(dt2 != dt1);

    auto dt3{DateTime{Date::today(), Time{hours_t{12}, minutes_t{12}}}};
    auto dt4{DateTime{Date::today(), Time{hours_t{12}, minutes_t{13}}}};

    EXPECT_TRUE(dt3 < dt4);
    EXPECT_TRUE(dt3 <= dt4);

    EXPECT_TRUE(dt4 > dt3);
    EXPECT_TRUE(dt4 >= dt3);

    EXPECT_FALSE(dt3 == dt4);
    EXPECT_FALSE(dt4 == dt3);

    EXPECT_TRUE(dt3 != dt4);
    EXPECT_TRUE(dt4 != dt3);

    auto dt5{DateTime{Date::today(), Time::now()}};
    auto dt6{DateTime{Date::today(), Time::now()}};

    EXPECT_FALSE(dt5 > dt6);
    EXPECT_TRUE(dt5 >= dt6);

    EXPECT_FALSE(dt5 < dt6);
    EXPECT_TRUE(dt5 <= dt6);

    EXPECT_TRUE(dt5 == dt6);
    EXPECT_FALSE(dt5 != dt6);

    EXPECT_TRUE(dt6 == dt5);
    EXPECT_FALSE(dt6 != dt5);
}

TEST(TestDateTime, OperatorPlusNoOverflow) {
    auto dt{DateTime{Date(year{2027}, month{10}, day{11}),
                     Time{hours_t{12}, minutes_t{12}}}};

    auto interval{hbt::mods::Interval(hbt::mods::Duration::Units{
        .years = 1, .months = 1, .days = 1, .hours = 1, .minutes = 1})};

    auto res{dt + interval};

    EXPECT_EQ(res.getDate(), Date(year(2028), month(11), day(12)));
    EXPECT_EQ(res.getTime(), Time(hours_t(13), minutes_t(13)));
}

TEST(TestDateTime, OperatorPlusOverflow) {
    auto dt{DateTime{Date(year{2027}, month{10}, day{11}),
                     Time{hours_t{22}, minutes_t{50}}}};

    auto interval{hbt::mods::Interval(hbt::mods::Duration::Units{
        .years = 1, .months = 1, .days = 1, .hours = 1, .minutes = 10})};

    auto res{dt + interval};

    EXPECT_EQ(res.getDate(), Date(year(2028), month(11), day(13)));
    EXPECT_EQ(res.getTime(), Time(hours_t(0), minutes_t(0)));
}

TEST(TestDateTime, OperatorPlusMonthHandling) {
    auto dt{DateTime{Date(year{2027}, month{10}, day{31}),
                     Time{hours_t{22}, minutes_t{0}}}}; // october has 31 days,
                                                        // november has 30

    auto wrapAroundInterval{hbt::mods::Interval(
        hbt::mods::Duration::Units{
            .years = 0, .months = 1, .days = 0, .hours = 0, .minutes = 0},
        mods::Interval::MonthHandling::WRAP_AROUND)};

    auto res1{dt + wrapAroundInterval};

    EXPECT_EQ(res1.getDate(), Date(year(2027), month(12), day(1)));
    EXPECT_EQ(res1.getTime(), Time(hours_t(22), minutes_t(0)));

    auto cutOffInterval{hbt::mods::Interval(
        hbt::mods::Duration::Units{
            .years = 0, .months = 1, .days = 0, .hours = 0, .minutes = 0},
        mods::Interval::MonthHandling::CUT_OFF)};

    auto res2{dt + cutOffInterval};

    EXPECT_EQ(res2.getDate(), Date(year(2027), month(11), day(30)));
    EXPECT_EQ(res2.getTime(), Time(hours_t(22), minutes_t(0)));
}

TEST(TestDateTime, GetDiff) {
    auto dt1{DateTime(Date::today(), Time::now())};
    EXPECT_TRUE(hbt::mods::Duration{} == DateTime::getDiff(dt1, dt1));

    auto dt2{DateTime(Date::today(), Time(hours_t(12), minutes_t(12)))};
    auto dt3{DateTime(Date::today(), Time(hours_t(13), minutes_t(13)))};
    EXPECT_TRUE(hbt::mods::Duration(hbt::mods::Duration::Units{
                    .hours = 1, .minutes = 1}) == DateTime::getDiff(dt2, dt3));
    EXPECT_TRUE(hbt::mods::Duration(hbt::mods::Duration::Units{
                    .hours = 1, .minutes = 1}) == DateTime::getDiff(dt3, dt2));

    auto dt4{DateTime(Date(year(2025), month(11), day(10)), Time::now())};
    auto dt5{DateTime(Date(year(2026), month(12), day(11)), Time::now())};
    EXPECT_TRUE(hbt::mods::Duration(hbt::mods::Duration::Units{
                    .years = 1, .months = 1, .days = 1}) ==
                DateTime::getDiff(dt4, dt5))
        << DateTime::getDiff(dt4, dt5).toISO8601String() << '\n';
    EXPECT_TRUE(hbt::mods::Duration(hbt::mods::Duration::Units{
                    .years = 1, .months = 1, .days = 1}) ==
                DateTime::getDiff(dt5, dt4));

    auto dt6{DateTime(Date(year(2025), month(11), day(10)),
                      Time(hours_t(12), minutes_t(12)))};
    auto dt7{DateTime(Date(year(2026), month(12), day(11)),
                      Time(hours_t(13), minutes_t(13)))};
    EXPECT_TRUE(
        hbt::mods::Duration(hbt::mods::Duration::Units{
            .years = 1, .months = 1, .days = 1, .hours = 1, .minutes = 1}) ==
        DateTime::getDiff(dt6, dt7));
    EXPECT_TRUE(
        hbt::mods::Duration(hbt::mods::Duration::Units{
            .years = 1, .months = 1, .days = 1, .hours = 1, .minutes = 1}) ==
        DateTime::getDiff(dt7, dt6));
}
} // namespace test::mods
