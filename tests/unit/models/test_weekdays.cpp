#include <gtest/gtest.h>

#include <datetime.hpp>
#include <weekdays.hpp>

namespace test::mods {

using hbt::mods::DateTime;
using hbt::mods::Weekdays;

TEST(WeekdaysTest, ConstructsEmptyByDefaultVector) {
    std::vector<DateTime::weekday_t> input{};

    Weekdays weekdays{input};

    auto days{weekdays.getDays()};
    EXPECT_TRUE(days.none());
}

TEST(WeekdaysTest, ConstructsFromVector) {
    std::vector<DateTime::weekday_t> input{DateTime::weekday_t::MONDAY,
                                           DateTime::weekday_t::WEDNESDAY};

    Weekdays weekdays{input};

    EXPECT_TRUE(weekdays.containsWeekday(DateTime::weekday_t::MONDAY));
    EXPECT_TRUE(weekdays.containsWeekday(DateTime::weekday_t::WEDNESDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::FRIDAY));
}

TEST(WeekdaysTest, ConstructsFromBitset) {
    Weekdays::days_t days;
    days.set(static_cast<size_t>(DateTime::weekday_t::TUESDAY));
    days.set(static_cast<size_t>(DateTime::weekday_t::THURSDAY));

    Weekdays weekdays{days};

    EXPECT_TRUE(weekdays.containsWeekday(DateTime::weekday_t::TUESDAY));
    EXPECT_TRUE(weekdays.containsWeekday(DateTime::weekday_t::THURSDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::MONDAY));
}

TEST(WeekdaysTest, ValidatesStringLengthCorrect) {
    std::string valid(Weekdays::daysCount, '0');

    EXPECT_NO_THROW(Weekdays weekdays{valid});
}

TEST(WeekdaysTest, RejectsInvalidStringLengthTooShort) {
    std::string invalid = "000";

    EXPECT_THROW(Weekdays weekdays{invalid}, std::invalid_argument);
}

TEST(WeekdaysTest, RejectsInvalidStringLengthTooLong) {
    std::string invalid(Weekdays::daysCount + 1, '0');

    EXPECT_THROW(Weekdays weekdays{invalid}, std::invalid_argument);
}

TEST(WeekdaysTest, ContainsWeekdayMatchesBitset) {
    std::string pattern(Weekdays::daysCount, '0');
    pattern[static_cast<size_t>(DateTime::weekday_t::FRIDAY)] = '1';

    Weekdays weekdays{pattern};

    EXPECT_TRUE(weekdays.containsWeekday(DateTime::weekday_t::FRIDAY));
    EXPECT_FALSE(weekdays.containsWeekday(DateTime::weekday_t::MONDAY));
}

TEST(WeekdaysTest, ToJSONReturnsString) {
    std::vector<DateTime::weekday_t> input{DateTime::weekday_t::MONDAY,
                                           DateTime::weekday_t::FRIDAY};

    Weekdays weekdays{input};

    auto json{weekdays.toJSON()};

    EXPECT_TRUE(json.is_string());
}

TEST(WeekdaysTest, FromJSONValid) {
    std::vector<DateTime::weekday_t> input{DateTime::weekday_t::MONDAY,
                                           DateTime::weekday_t::FRIDAY};

    Weekdays original{input};

    auto json{original.toJSON()};
    auto restored{Weekdays::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_TRUE(restored->containsWeekday(DateTime::weekday_t::MONDAY));
    EXPECT_TRUE(restored->containsWeekday(DateTime::weekday_t::FRIDAY));
}

TEST(WeekdaysTest, FromJSONInvalidType) {
    nlohmann::json json = 123; // not string

    auto result{Weekdays::fromJSON(json)};
    EXPECT_FALSE(result.has_value());
}

TEST(WeekdaysTest, FromJSONInvalidStringLength) {
    nlohmann::json json = std::string("too_short");

    auto result{Weekdays::fromJSON(json)};
    EXPECT_FALSE(result.has_value());
}
} // namespace test::mods
