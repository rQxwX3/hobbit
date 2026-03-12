#include <entry.hpp>
#include <user.hpp>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <vector>

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

    auto json{original.toJSON()};
    EXPECT_EQ(json["name"], "alice");

    auto restored{User::fromJSON(json)};
    EXPECT_EQ(restored.getName(), "alice");
}

TEST(EntryTest, GetSetTitle) {
    using hbt::mods::Entry, hbt::mods::Weekday, hbt::mods::Daypart;

    auto entry{Entry("todo", {{Weekday::MONDAY, Daypart::MORNING}})};
    EXPECT_EQ(entry.getTitle(), "todo");

    entry.setTitle("ornottodo");
    EXPECT_EQ(entry.getTitle(), "ornottodo");
}

TEST(EntryTest, ToggleIsCompleted) {
    using hbt::mods::Entry, hbt::mods::Weekday, hbt::mods::Daypart;

    auto entry{Entry("todo", {{Weekday::MONDAY, Daypart::MORNING}})};
    EXPECT_EQ(entry.isCompleted(), false);

    entry.toggleIsCompleted();
    EXPECT_EQ(entry.isCompleted(), true);
}

TEST(EntryTest, ToFromJSON) {
    using hbt::mods::Entry, hbt::mods::Occurence, hbt::mods::Weekday,
        hbt::mods::Daypart;

    auto occurences{
        std::vector<Occurence>(1, {Weekday::MONDAY, Daypart::MORNING})};

    auto jsonOccurences{nlohmann::json::array()};
    for (const auto &occ : occurences) {
        jsonOccurences.emplace_back(occ.toJSON());
    }

    auto original{Entry("todo", occurences)};

    auto json{original.toJSON()};
    EXPECT_EQ(json["title"], "todo");

    EXPECT_EQ(json["occurences"].size(), 1);
    EXPECT_EQ(jsonOccurences.size(), 1);

    EXPECT_EQ(json["occurences"][0]["weekday"], jsonOccurences[0]["weekday"]);
    EXPECT_EQ(json["occurences"][0]["daypart"], jsonOccurences[0]["daypart"]);

    auto restored{Entry::fromJSON(json)};
    const auto &restoredOccurences{restored.getOccurences()};
    EXPECT_EQ(restored.getTitle(), "todo");

    EXPECT_EQ(jsonOccurences.size(), 1);
    EXPECT_EQ(restoredOccurences.size(), 1);

    EXPECT_EQ(restoredOccurences[0].getWeekday(), occurences[0].getWeekday());
    EXPECT_EQ(restoredOccurences[0].getDaypart(), occurences[0].getDaypart());
}

TEST(OccurenceTest, ToFromJSON) {
    using hbt::mods::Occurence, hbt::mods::Weekday, hbt::mods::Daypart;

    auto original{Occurence{Weekday::MONDAY, Daypart::MORNING}};
    auto json{original.toJSON()};
    EXPECT_EQ(json["weekday"], Weekday::MONDAY);
    EXPECT_EQ(json["daypart"], Daypart::MORNING);

    auto restored{Occurence::fromJSON(json)};
    EXPECT_EQ(restored.getWeekday(), Weekday::MONDAY);
    EXPECT_EQ(restored.getDaypart(), Daypart::MORNING);
}
