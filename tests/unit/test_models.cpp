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

    auto json{original.toJSON()};
    EXPECT_EQ(json["name"], "alice");

    auto restored{User::fromJSON(json)};
    EXPECT_EQ(restored.getName(), "alice");
}

TEST(OccurenceTest, ToFromJSON) {
    using hbt::mods::Occurence, hbt::mods::Daypart;

    auto original{Occurence{std::chrono::weekday(), Daypart::MORNING}};
    auto json{original.toJSON()};
    EXPECT_EQ(json["weekday"], std::chrono::weekday().iso_encoding());
    EXPECT_EQ(json["daypart"], Daypart::MORNING);

    auto restored{Occurence::fromJSON(json)};
    EXPECT_EQ(restored.getWeekday(), std::chrono::weekday());
    EXPECT_EQ(restored.getDaypart(), Daypart::MORNING);
}

TEST(EntryTest, GetSetTitle) {
    using hbt::mods::Entry, hbt::mods::Daypart;

    auto entry{Entry("todo", {{std::chrono::weekday(), Daypart::MORNING}})};
    EXPECT_EQ(entry.getTitle(), "todo");

    entry.setTitle("ornottodo");
    EXPECT_EQ(entry.getTitle(), "ornottodo");
}

TEST(EntryTest, ToggleIsCompleted) {
    using hbt::mods::Entry, hbt::mods::Daypart;

    auto entry{Entry("todo", {{std::chrono::weekday(), Daypart::MORNING}})};
    EXPECT_EQ(entry.isCompleted(), false);

    entry.toggleIsCompleted();
    EXPECT_EQ(entry.isCompleted(), true);
}

TEST(EntryTest, ToFromJSON) {
    using hbt::mods::Entry, hbt::mods::Occurence, hbt::mods::Daypart;

    auto occurences{
        std::vector<Occurence>({{std::chrono::weekday(), Daypart::MORNING},
                                {std::chrono::weekday(), Daypart::AFTERNOON}})};

    auto jsonOccurences{nlohmann::json::array()};
    for (const auto &occ : occurences) {
        jsonOccurences.emplace_back(occ.toJSON());
    }

    auto original{Entry("todo", occurences)};
    auto json{original.toJSON()};
    EXPECT_EQ(json["title"], "todo");
    EXPECT_THAT(json["occurrences"],
                testing::UnorderedElementsAre(occurences[0].toJSON(),
                                              occurences[1].toJSON()));
    auto restored{Entry::fromJSON(json)};
    const auto &restoredOccurences{restored.getOccurences()};
    EXPECT_EQ(restored.getTitle(), "todo");
    EXPECT_THAT(restoredOccurences,
                testing::UnorderedElementsAre(occurences[0], occurences[1]));
}
} // namespace test::mods
