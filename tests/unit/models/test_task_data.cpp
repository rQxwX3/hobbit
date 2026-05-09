#include <gtest/gtest.h>

#include <task_data.hpp>

namespace test::mods {
using hbt::mods::TaskData;

TEST(TaskDataTest, CtorThrowsOnEmptyTitle) {
    EXPECT_THROW(TaskData("", false), std::invalid_argument);
}

TEST(TaskDataTest, SetterThrowsOnEmptyTitle) {
    EXPECT_THROW(TaskData("Title", false).setTitle(""), std::invalid_argument);
}

TEST(TaskDataTest, NotCompletedByDefault) {
    EXPECT_FALSE(TaskData("Title").isCompleted());
}

TEST(TaskDataTest, ConstructionAndGetters) {
    auto task{TaskData("Test", false)};

    EXPECT_EQ(task.getTitle(), "Test");
    EXPECT_FALSE(task.isCompleted());
}

TEST(TaskDataTest, Setters) {
    auto task{TaskData("Test", false)};

    task.setTitle("Updated");
    EXPECT_EQ(task.getTitle(), "Updated");

    task.setCompleted(true);
    EXPECT_TRUE(task.isCompleted());
}

TEST(TaskDataTest, JSONRoundTrip) {
    auto original{TaskData("Test", true)};

    auto json = original.toJSON();
    auto restored{TaskData::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getTitle(), original.getTitle());
    EXPECT_EQ(restored->isCompleted(), original.isCompleted());
}

TEST(TaskDataTest, FromJSONFailsOnInvalidJSON) {
    auto json = nlohmann::json();

    /* empty json */
    EXPECT_FALSE(TaskData::fromJSON(json));

    /* missing title */
    EXPECT_FALSE(TaskData::fromJSON({{"completed", false}}).has_value());

    /* empty title */
    EXPECT_FALSE(TaskData::fromJSON({{"title", ""}, {"completed", false}}));

    /* missing completed */
    EXPECT_FALSE(TaskData::fromJSON({{"title", "title"}}));
}
} // namespace test::mods
