#include <gtest/gtest.h>

#include <datetime.hpp>
#include <weekdays.hpp>

namespace test::mods {
using hbt::mods::DateTime;
using hbt::mods::Weekdays;

TEST(WeekdaysTest, ConstructsEmptyByDefault) {
    auto weekdays(Weekdays(Weekdays::days_t{}));

    auto days{weekdays.getDays()};
    EXPECT_TRUE(days.none());
}

TEST(WeekdaysTest, ConstructsFromVector) {
    auto vector{std::vector<DateTime::weekday_t>{
        DateTime::weekday_t::MONDAY, DateTime::weekday_t::WEDNESDAY}};

    auto weekdays{Weekdays(vector)};

    EXPECT_TRUE(weekdays.containsWeekday(DateTime::weekday_t::MONDAY));
    EXPECT_TRUE(weekdays.containsWeekday(DateTime::weekday_t::WEDNESDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::TUESDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::THURSDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::FRIDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::SATURDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::SUNDAY));
}

TEST(WeekdaysTest, ConstructsFromBitset) {
    auto days{Weekdays::days_t{}};
    days.set(static_cast<size_t>(DateTime::weekday_t::TUESDAY));
    days.set(static_cast<size_t>(DateTime::weekday_t::THURSDAY));

    auto weekdays{Weekdays(days)};

    EXPECT_TRUE(weekdays.containsWeekday(DateTime::weekday_t::TUESDAY));
    EXPECT_TRUE(weekdays.containsWeekday(DateTime::weekday_t::THURSDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::MONDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::WEDNESDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::FRIDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::SATURDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::SUNDAY));
}

TEST(WeekdaysTest, JSONRoundTrip) {
    auto original{
        Weekdays({DateTime::weekday_t::MONDAY, DateTime::weekday_t::FRIDAY})};

    auto json = original.toJSON();
    auto restored{Weekdays::fromJSON(json)};

    ASSERT_TRUE(restored);

    EXPECT_TRUE(restored->containsWeekday(DateTime::weekday_t::MONDAY));
    EXPECT_TRUE(restored->containsWeekday(DateTime::weekday_t::FRIDAY));
    EXPECT_FALSE(restored->containsWeekday(DateTime::weekday_t::SUNDAY));
}

TEST(WeekdaysTest, FromJSONFailsOnInvalidJSON) {
    auto result{Weekdays::fromJSON("se777en")};
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Weekdays::Error::JSONInvalidString);

    result = Weekdays::fromJSON("1234567");
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Weekdays::Error::JSONInvalidString);

    result = Weekdays::fromJSON("11110000");
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Weekdays::Error::JSONInvalidString);

    /* empty string */
    result = Weekdays::fromJSON("");
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Weekdays::Error::JSONInvalidString);

    /* empty JSON */
    result = Weekdays::fromJSON({});
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), Weekdays::Error::JSONNotString);
}
} // namespace test::mods
