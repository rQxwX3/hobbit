#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <entry.hpp>

namespace test::mods {
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
