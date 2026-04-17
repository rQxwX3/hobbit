#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <entry.hpp>

namespace test::mods {
using hbt::mods::Entry, hbt::mods::Occurrence, hbt::mods::DateTime,
    hbt::mods::Interval;

TEST(EntryTest, GetSetTitle) {
    auto entry{Entry("todo", {})};
    EXPECT_EQ(entry.getTitle(), "todo");

    entry.setTitle("ornottodo");
    EXPECT_EQ(entry.getTitle(), "ornottodo");
}

TEST(EntryTest, GetSetIsCompleted) {
    auto entry{Entry("todo", {})};
    EXPECT_EQ(entry.isCompleted(), false);

    entry.setIsCompleted(true);
    EXPECT_EQ(entry.isCompleted(), true);
}

TEST(EntryTest, GetSetOccurrences) {
    auto occurrences1{std::vector<Occurrence>{
        Occurrence{DateTime::today()},
        Occurrence{DateTime::today() + Interval::days(1)}}};
    auto occurrences2{std::vector<Occurrence>{
        Occurrence{DateTime::today() + Interval::weeks(5)},
        Occurrence{DateTime::today()}}};

    auto entry{Entry{"todo", occurrences1}};
    EXPECT_THAT(entry.getOccurrences(),
                ::testing::UnorderedElementsAreArray(occurrences1));

    entry.setOccurrences(occurrences2);
    EXPECT_THAT(entry.getOccurrences(),
                ::testing::UnorderedElementsAreArray(occurrences2));
}

TEST(EntryTest, ToFromJSON) {
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
                ::testing::UnorderedElementsAreArray(jsonOccurrences));

    auto restored{Entry::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());

    const auto &restoredOccurrences{restored.value().getOccurrences()};
    EXPECT_EQ(restored.value().getTitle(), "todo");
    EXPECT_THAT(restoredOccurrences,
                ::testing::UnorderedElementsAreArray(occurrences));
}

TEST(EntryTest, IsForDate) {
    auto occurrences{std::vector<Occurrence>{
        Occurrence{DateTime::today(), Interval::years(1)},
        Occurrence{DateTime::today(), Interval::months(1)},
    }};

    auto entry{Entry{"todo", occurrences}};

    EXPECT_TRUE(entry.isForDate(DateTime::today()));
    EXPECT_TRUE(entry.isForDate(DateTime::today() + Interval::years(1)));
    EXPECT_TRUE(entry.isForDate(DateTime::today() + Interval::months(1)));

    EXPECT_FALSE(entry.isForDate(DateTime::today() + Interval::weeks(1)));
    EXPECT_FALSE(entry.isForDate(DateTime::today() + Interval::days(1)));
}
} // namespace test::mods
