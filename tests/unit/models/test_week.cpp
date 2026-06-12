#include <gtest/gtest.h>

#include <datetime.hpp>
#include <week.hpp>

namespace test::mods {
using hbt::mods::DateTime;
using hbt::mods::Week;

TEST(WeekTest, ConstructsEmptyByDefault) {
    auto days{Week().getDays()};
    EXPECT_TRUE(days.none());
}

TEST(WeekTest, ConstructsFromVector) {
    auto vector{std::vector<DateTime::weekday_t>{
        DateTime::weekday_t::MONDAY, DateTime::weekday_t::WEDNESDAY}};

    auto week{Week(vector)};

    EXPECT_TRUE(week.containsWeekday(DateTime::weekday_t::MONDAY));
    EXPECT_TRUE(week.containsWeekday(DateTime::weekday_t::WEDNESDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::TUESDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::THURSDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::FRIDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::SATURDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::SUNDAY));
}

TEST(WeekTest, ConstructsFromBitset) {
    auto days{Week::days_t{}};
    days.set(static_cast<size_t>(DateTime::weekday_t::TUESDAY));
    days.set(static_cast<size_t>(DateTime::weekday_t::THURSDAY));

    auto week{Week(days)};

    EXPECT_TRUE(week.containsWeekday(DateTime::weekday_t::TUESDAY));
    EXPECT_TRUE(week.containsWeekday(DateTime::weekday_t::THURSDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::MONDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::WEDNESDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::FRIDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::SATURDAY));
    EXPECT_FALSE(week.containsWeekday(DateTime::weekday_t::SUNDAY));
}

TEST(WeekTest, JSONRoundTrip) {
    auto original{
        Week({DateTime::weekday_t::MONDAY, DateTime::weekday_t::FRIDAY})};

    auto json = original.toJSON();
    auto restored{Week::fromJSON(json)};

    ASSERT_TRUE(restored);

    EXPECT_TRUE(restored->containsWeekday(DateTime::weekday_t::MONDAY));
    EXPECT_TRUE(restored->containsWeekday(DateTime::weekday_t::FRIDAY));
    EXPECT_FALSE(restored->containsWeekday(DateTime::weekday_t::SUNDAY));
}

TEST(WeekTest, FromJSONFailsOnInvalidJSON) {
    auto result{Week::fromJSON("se777en")};
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Week::Error::JSONInvalidString);

    result = Week::fromJSON("1234567");
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Week::Error::JSONInvalidString);

    result = Week::fromJSON("11110000");
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Week::Error::JSONInvalidString);

    /* empty string */
    result = Week::fromJSON("");
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Week::Error::JSONInvalidString);

    /* empty JSON */
    result = Week::fromJSON({});
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Week::Error::JSONNotString);
}
} // namespace test::mods
