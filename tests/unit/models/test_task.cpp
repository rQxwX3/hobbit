#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <task.hpp>

namespace test::mods {
using hbt::mods::Task, hbt::mods::Occurrence, hbt::mods::DateTime,
    hbt::mods::Interval;

TEST(TaskTest, GetSetTitle) {
    auto task{Task("todo", {})};
    EXPECT_EQ(task.getTitle(), "todo");

    task.setTitle("ornottodo");
    EXPECT_EQ(task.getTitle(), "ornottodo");
}

TEST(TaskTest, GetSetIsCompleted) {
    auto task{Task("todo", {})};
    EXPECT_EQ(task.isCompleted(), false);

    task.setIsCompleted(true);
    EXPECT_EQ(task.isCompleted(), true);
}

TEST(TaskTest, GetSetOccurrences) {
    auto occurrences1{std::vector<Occurrence>{
        Occurrence{DateTime::today()},
        Occurrence{DateTime::today() + Interval::days(1)}}};
    auto occurrences2{std::vector<Occurrence>{
        Occurrence{DateTime::today() + Interval::weeks(5)},
        Occurrence{DateTime::today()}}};

    auto task{Task{"todo", occurrences1}};
    EXPECT_THAT(task.getOccurrences(),
                ::testing::UnorderedElementsAreArray(occurrences1));

    task.setOccurrences(occurrences2);
    EXPECT_THAT(task.getOccurrences(),
                ::testing::UnorderedElementsAreArray(occurrences2));
}

TEST(TaskTest, ToFromJSON) {
    auto occurrences{std::vector<Occurrence>({{}, {}})};

    auto jsonOccurrences = nlohmann::json::array();
    for (const auto &occ : occurrences) {
        jsonOccurrences.push_back(occ.toJSON());
    }

    auto original{Task("todo", occurrences)};

    auto json = original.toJSON();
    ASSERT_EQ(std::string(json.type_name()), "object");

    EXPECT_EQ(json["title"], "todo");
    EXPECT_EQ(json["occurrences"].size(), occurrences.size());
    EXPECT_THAT(json["occurrences"],
                ::testing::UnorderedElementsAreArray(jsonOccurrences));

    auto restored{Task::fromJSON(json)};
    ASSERT_TRUE(restored.has_value());

    const auto &restoredOccurrences{restored.value().getOccurrences()};
    EXPECT_EQ(restored.value().getTitle(), "todo");
    EXPECT_THAT(restoredOccurrences,
                ::testing::UnorderedElementsAreArray(occurrences));
}

TEST(TaskTest, IsForDate) {
    auto occurrences{std::vector<Occurrence>{
        Occurrence{DateTime::today(), Interval::years(1)},
        Occurrence{DateTime::today(), Interval::months(1)},
    }};

    auto task{Task{"todo", occurrences}};

    EXPECT_TRUE(task.isForDate(DateTime::today()));
    EXPECT_TRUE(task.isForDate(DateTime::today() + Interval::years(1)));
    EXPECT_TRUE(task.isForDate(DateTime::today() + Interval::months(1)));

    EXPECT_FALSE(task.isForDate(DateTime::today() + Interval::weeks(1)));
    EXPECT_FALSE(task.isForDate(DateTime::today() + Interval::days(1)));
}
} // namespace test::mods
