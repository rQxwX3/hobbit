#include <entry.hpp>
#include <user.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

namespace test::mods {
TEST(UserTest, GetSetName) {
    using hbt::mods::User;

    auto user{User{"alice"}};
    EXPECT_EQ(user.getName(), "alice");

    user.setName("bob");
    EXPECT_EQ(user.getName(), "bob");
}

TEST(UserTest, ToFromJSON) {
    using hbt::mods::User;

    auto original{User{"alice"}};

    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");
    EXPECT_EQ(json["name"].get<std::string>(), "alice");

    auto restored{User::fromJSON(json)};
    EXPECT_EQ(restored.getName(), "alice");
}

TEST(DateTest, ToFromYMDString) {
    using hbt::mods::Date;

    auto original{Date{}};

    auto ymdString{original.toYMDString()};

    auto restored{Date::fromYMDString(ymdString)};
    EXPECT_EQ(restored.getYear(), original.getYear());
    EXPECT_EQ(restored.getMonth(), original.getMonth());
    EXPECT_EQ(restored.getDay(), original.getDay());
}

TEST(DurationUnitsTest, EmptyOnConstruction) {
    auto durationUnits{hbt::mods::DurationUnits{}};

    ASSERT_EQ(durationUnits.getYears(), 0);
    ASSERT_EQ(durationUnits.getMonths(), 0);
    ASSERT_EQ(durationUnits.getWeeks(), 0);
    ASSERT_EQ(durationUnits.getDays(), 0);
    ASSERT_EQ(durationUnits.getHours(), 0);
    ASSERT_EQ(durationUnits.getMinutes(), 0);
}

TEST(DurationUnitsTest, IsZero) {
    auto durationUnits{hbt::mods::DurationUnits{}};
    EXPECT_TRUE(durationUnits.isZero());

    durationUnits.addDays(1);
    EXPECT_FALSE(durationUnits.isZero());
}

TEST(DurationUnitsTest, AddGetUnits) {
    auto durationUnits{hbt::mods::DurationUnits{}};

    durationUnits.addYears(1);
    EXPECT_EQ(durationUnits.getYears(), 1);

    durationUnits.addMonths(1);
    EXPECT_EQ(durationUnits.getMonths(), 1);

    durationUnits.addWeeks(1);
    EXPECT_EQ(durationUnits.getWeeks(), 1);

    durationUnits.addDays(1);
    EXPECT_EQ(durationUnits.getDays(), 1);

    durationUnits.addHours(1);
    EXPECT_EQ(durationUnits.getHours(), 1);

    durationUnits.addMinutes(1);
    EXPECT_EQ(durationUnits.getMinutes(), 1);
}

TEST(DurationUnitsTest, AddAutoChangesUnits) {
    auto monthsToYears1{hbt::mods::DurationUnits{}};
    monthsToYears1.addMonths(hbt::mods::DurationUnits::monthsInYear);
    EXPECT_EQ(monthsToYears1.getMonths(), 0);
    EXPECT_EQ(monthsToYears1.getYears(), 1);

    auto monthsToYears2{hbt::mods::DurationUnits{}};
    monthsToYears2.addMonths(hbt::mods::DurationUnits::monthsInYear * 2);
    EXPECT_EQ(monthsToYears2.getMonths(), 0);
    EXPECT_EQ(monthsToYears2.getYears(), 2);

    auto monthsToYears3{hbt::mods::DurationUnits{}};
    monthsToYears3.addMonths(hbt::mods::DurationUnits::monthsInYear + 1);
    EXPECT_EQ(monthsToYears3.getMonths(), 1);
    EXPECT_EQ(monthsToYears3.getYears(), 1);

    auto weeksToMonths1{hbt::mods::DurationUnits{}};
    weeksToMonths1.addWeeks(hbt::mods::DurationUnits::weeksInMonth);
    EXPECT_EQ(weeksToMonths1.getWeeks(), 0);
    EXPECT_EQ(weeksToMonths1.getMonths(), 1);

    auto weeksToMonths2{hbt::mods::DurationUnits{}};
    weeksToMonths2.addWeeks(hbt::mods::DurationUnits::weeksInMonth * 3);
    EXPECT_EQ(weeksToMonths2.getWeeks(), 0);
    EXPECT_EQ(weeksToMonths2.getMonths(), 3);

    auto weeksToMonths3{hbt::mods::DurationUnits{}};
    weeksToMonths3.addWeeks(hbt::mods::DurationUnits::weeksInMonth + 2);
    EXPECT_EQ(weeksToMonths3.getWeeks(), 2);
    EXPECT_EQ(weeksToMonths3.getMonths(), 1);

    auto daysToWeek1{hbt::mods::DurationUnits{}};
    daysToWeek1.addDays(hbt::mods::DurationUnits::daysInWeek);
    EXPECT_EQ(daysToWeek1.getDays(), 0);
    EXPECT_EQ(daysToWeek1.getWeeks(), 1);

    auto daysToWeek2{hbt::mods::DurationUnits{}};
    daysToWeek2.addDays(hbt::mods::DurationUnits::daysInWeek * 3);
    EXPECT_EQ(daysToWeek2.getDays(), 0);
    EXPECT_EQ(daysToWeek2.getWeeks(), 3);

    auto daysToWeek3{hbt::mods::DurationUnits{}};
    daysToWeek3.addDays(hbt::mods::DurationUnits::daysInWeek + 4);
    EXPECT_EQ(daysToWeek3.getDays(), 4);
    EXPECT_EQ(daysToWeek3.getWeeks(), 1);

    auto hoursToDays1{hbt::mods::DurationUnits{}};
    hoursToDays1.addHours(hbt::mods::DurationUnits::hoursInDay);
    EXPECT_EQ(hoursToDays1.getHours(), 0);
    EXPECT_EQ(hoursToDays1.getDays(), 1);

    auto hoursToDays2{hbt::mods::DurationUnits{}};
    hoursToDays2.addHours(hbt::mods::DurationUnits::hoursInDay * 5);
    EXPECT_EQ(hoursToDays2.getHours(), 0);
    EXPECT_EQ(hoursToDays2.getDays(), 5);

    auto hoursToDays3{hbt::mods::DurationUnits{}};
    hoursToDays3.addHours(hbt::mods::DurationUnits::hoursInDay + 15);
    EXPECT_EQ(hoursToDays3.getHours(), 15);
    EXPECT_EQ(hoursToDays3.getDays(), 1);

    auto minutesToHours1{hbt::mods::DurationUnits{}};
    minutesToHours1.addMinutes(hbt::mods::DurationUnits::minutesInHour);
    EXPECT_EQ(minutesToHours1.getMinutes(), 0);
    EXPECT_EQ(minutesToHours1.getHours(), 1);

    auto minutesToHours2{hbt::mods::DurationUnits{}};
    minutesToHours2.addMinutes(hbt::mods::DurationUnits::minutesInHour * 6);
    EXPECT_EQ(minutesToHours2.getMinutes(), 0);
    EXPECT_EQ(minutesToHours2.getHours(), 6);

    auto minutesToHours3{hbt::mods::DurationUnits{}};
    minutesToHours3.addMinutes(hbt::mods::DurationUnits::minutesInHour + 30);
    EXPECT_EQ(minutesToHours3.getMinutes(), 30);
    EXPECT_EQ(minutesToHours3.getHours(), 1);
}

TEST(DurationUnitsTest, OperatorPlus) {
    auto first{hbt::mods::DurationUnits{}};
    first.addYears(1);
    first.addMonths(1);
    first.addWeeks(1);
    first.addDays(1);
    first.addHours(1);
    first.addMinutes(1);

    auto second{hbt::mods::DurationUnits{}};
    second.addYears(1);
    second.addMonths(2);
    second.addWeeks(3);
    second.addDays(4);
    second.addHours(5);
    second.addMinutes(6);

    auto result{first + second};
    EXPECT_EQ(result.getYears(), 2);
    EXPECT_EQ(result.getMonths(), 3);
    EXPECT_EQ(result.getWeeks(), 4);
    EXPECT_EQ(result.getDays(), 5);
    EXPECT_EQ(result.getHours(), 6);
    EXPECT_EQ(result.getMinutes(), 7);
}

TEST(DurationUnitsTest, ComparisonOperators) {
    auto first{hbt::mods::DurationUnits{}};
    auto second{hbt::mods::DurationUnits{}};
    EXPECT_TRUE(first == second);
    EXPECT_TRUE(second == first);

    first.addDays(1);
    EXPECT_TRUE(first > second);
    EXPECT_TRUE(second < first);

    second.addDays(2);
    EXPECT_TRUE(second > first);
    EXPECT_TRUE(first < second);

    first.addMonths(1);
    EXPECT_TRUE(first > second);
    EXPECT_TRUE(second < first);

    second.addMonths(1);
    first.addDays(1);
    EXPECT_TRUE(first == second);
    EXPECT_TRUE(second == first);
}

TEST(DurationUnitsTest, ToFromISO8601String) {
    auto first{hbt::mods::DurationUnits{}};
    auto firstISO{first.toISO8601String()};
    EXPECT_EQ(firstISO, "PT0M");
    auto restored{hbt::mods::DurationUnits::fromISO8601String(firstISO)};
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addMinutes(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "PT1M");
    restored = hbt::mods::DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addHours(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "PT1H1M");
    restored = hbt::mods::DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addDays(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1DT1H1M");
    restored = hbt::mods::DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addWeeks(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1W1DT1H1M");
    restored = hbt::mods::DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addMonths(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1M1W1DT1H1M");
    restored = hbt::mods::DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);

    first.addYears(1);
    firstISO = first.toISO8601String();
    EXPECT_EQ(firstISO, "P1Y1M1W1DT1H1M");
    restored = hbt::mods::DurationUnits::fromISO8601String(firstISO);
    ASSERT_TRUE(restored.has_value());
    EXPECT_EQ(restored.value(), first);
}

TEST(OccurrenceTest, ToFromJSON) {
    using hbt::mods::Occurrence;

    auto original{Occurrence{}};
    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["date"], original.getDate().toYMDString());

    auto restored{Occurrence::fromJSON(json)};
    EXPECT_EQ(restored.getDate().toYMDString(),
              original.getDate().toYMDString());
    EXPECT_EQ(restored.getWeekday(), original.getWeekday());
}

// TEST(OccurrenceTest, IsForDateTrueForFutureDate) {
//     using hbt::mods::Occurrence, hbt::mods::Date;
//
//     auto date{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{16},
//     }};

// auto occurrence1{Occurrence{date, std::chrono::weeks{1}}};
// EXPECT_TRUE(occurrence1.isForDate(date + std::chrono::weeks{1}));
// EXPECT_TRUE(occurrence1.isForDate(date + std::chrono::weeks{2}));
// EXPECT_TRUE(occurrence1.isForDate(date + std::chrono::days{7}));
//
// auto occurrence2{Occurrence{date, std::chrono::months{1}}};
// EXPECT_TRUE(occurrence2.isForDate(date + std::chrono::months{1}));
// EXPECT_TRUE(occurrence2.isForDate(date + std::chrono::months{2}));
// EXPECT_TRUE(occurrence2.isForDate(date + std::chrono::months{12}));

// auto occurrence3{Occurrence{date, std::chrono::years{1}}};
// EXPECT_TRUE(occurrence3.isForDate(date + std::chrono::years{1}));
// EXPECT_TRUE(occurrence3.isForDate(date + std::chrono::years{2}));
// EXPECT_TRUE(occurrence3.isForDate(date + std::chrono::months{12}));
// }

// TEST(OccurrenceTest, IsForDateTrueForSelf) {
//     using hbt::mods::Occurrence, hbt::mods::Date;
//
//     auto date1{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{16},
//     }};
//
//     auto occurrence{Occurrence{date1, 7}};
//
//     EXPECT_TRUE(occurrence.isForDate(date1));
// }

// TEST(OccurrenceTest, IsForDateFalseForPastDate) {
//     using hbt::mods::Occurrence, hbt::mods::Date;
//
//     auto date1{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{23},
//     }};
//
//     auto date2{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{16},
//     }};
//
//     auto occurrence{Occurrence{date1, 7}};
//
//     EXPECT_FALSE(occurrence.isForDate(date2));
// }

// TEST(OccurrenceTest, IsForDateFalseForWrongDate) {
//     using hbt::mods::Occurrence, hbt::mods::Date;
//
//     auto date1{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{16},
//     }};
//
//     auto date2{Date{
//         std::chrono::year{2026},
//         std::chrono::month{3},
//         std::chrono::day{17},
//     }};
//
//     auto occurrence{Occurrence{date1, 7}};
//
//     EXPECT_FALSE(occurrence.isForDate(date2));
// }

TEST(EntryTest, GetSetTitle) {
    using hbt::mods::Entry;

    auto entry{Entry("todo", {})};
    EXPECT_EQ(entry.getTitle(), "todo");

    entry.setTitle("ornottodo");
    EXPECT_EQ(entry.getTitle(), "ornottodo");
}

TEST(EntryTest, SetIsCompleted) {
    using hbt::mods::Entry;

    auto entry{Entry("todo", {})};
    EXPECT_EQ(entry.isCompleted(), false);

    entry.setIsCompleted(true);
    EXPECT_EQ(entry.isCompleted(), true);
}

TEST(EntryTest, ToFromJSON) {
    using hbt::mods::Entry, hbt::mods::Occurrence;

    auto occurrences{std::vector<Occurrence>({{}, {}})};

    auto jsonOccurrences = nlohmann::json::array();
    for (const auto &occ : occurrences) {
        jsonOccurrences.push_back(occ.toJSON());
    }

    auto original{Entry("todo", occurrences)};

    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["title"], "todo");
    EXPECT_EQ(json["occurrences"].size(), occurrences.size());
    EXPECT_THAT(json["occurrences"],
                testing::UnorderedElementsAre(occurrences[0].toJSON(),
                                              occurrences[1].toJSON()));

    auto restored{Entry::fromJSON(json)};
    const auto &restoredOccurrences{restored.getOccurrences()};
    EXPECT_EQ(restored.getTitle(), "todo");
    // EXPECT_THAT(restoredOccurrences,
    //             testing::UnorderedElementsAre(occurrences[0],
    //             occurrences[1]));
}
} // namespace test::mods
