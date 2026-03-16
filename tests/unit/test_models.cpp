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

TEST(OccurrenceTest, IsForDateTrueForFutureDate) {
    using hbt::mods::Occurrence, hbt::mods::Date;

    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto date2{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{23},
    }};

    auto original{Occurrence{date1, 7}};

    EXPECT_TRUE(original.isForDate(date2));
}

TEST(OccurrenceTest, IsForDateTrueForSelf) {
    using hbt::mods::Occurrence, hbt::mods::Date;

    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto original{Occurrence{date1, 7}};

    EXPECT_TRUE(original.isForDate(date1));
}

TEST(OccurrenceTest, IsForDateFalseForPastDate) {
    using hbt::mods::Occurrence, hbt::mods::Date;

    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{23},
    }};

    auto date2{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto original{Occurrence{date1, 7}};

    EXPECT_FALSE(original.isForDate(date2));
}

TEST(OccurrenceTest, IsForDateFalseForWrongDate) {
    using hbt::mods::Occurrence, hbt::mods::Date;

    auto date1{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{16},
    }};

    auto date2{Date{
        std::chrono::year{2026},
        std::chrono::month{3},
        std::chrono::day{17},
    }};

    auto original{Occurrence{date1, 7}};

    EXPECT_FALSE(original.isForDate(date2));
}

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
    EXPECT_THAT(restoredOccurrences,
                testing::UnorderedElementsAre(occurrences[0], occurrences[1]));
}
} // namespace test::mods
