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

TEST(OccurrenceTest, ToFromJSON) {
    using hbt::mods::Occurrence, hbt::mods::Daypart;

    auto original{Occurrence{std::chrono::weekday(), Daypart::MORNING}};
    auto json{original.toJSON()};
    EXPECT_EQ(json["weekday"], std::chrono::weekday().iso_encoding());
    EXPECT_EQ(json["daypart"], Daypart::MORNING);

    auto restored{Occurrence::fromJSON(json)};
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

TEST(EntryTest, SetIsCompleted) {
    using hbt::mods::Entry, hbt::mods::Daypart;

    auto entry{Entry("todo", {{std::chrono::weekday(), Daypart::MORNING}})};
    EXPECT_EQ(entry.isCompleted(), false);

    entry.setIsCompleted(true);
    EXPECT_EQ(entry.isCompleted(), true);
}

TEST(EntryTest, ToFromJSON) {
    using hbt::mods::Entry, hbt::mods::Occurrence, hbt::mods::Daypart;

    auto occurrences{std::vector<Occurrence>(
        {{std::chrono::weekday(), Daypart::MORNING},
         {std::chrono::weekday(), Daypart::AFTERNOON}})};

    auto jsonOccurrences{nlohmann::json::array()};
    for (const auto &occ : occurrences) {
        jsonOccurrences.emplace_back(occ.toJSON());
    }

    auto original{Entry("todo", occurrences)};
    auto json{original.toJSON()};
    EXPECT_EQ(json["title"], "todo");
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
