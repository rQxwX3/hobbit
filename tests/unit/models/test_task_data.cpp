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

    ASSERT_TRUE(restored);

    EXPECT_EQ(restored->getTitle(), original.getTitle());
    EXPECT_EQ(restored->isCompleted(), original.isCompleted());
}

TEST(TaskDataTest, FromJSONFailsOnInvalidJSON) {
    auto json = nlohmann::json();

    /* empty json */
    auto result{TaskData::fromJSON(json)};
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::Error::JSONMissingRequiredField);

    /* missing title */
    result = TaskData::fromJSON({{"completed", false}});
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::Error::JSONMissingRequiredField);

    /* empty title */
    result = TaskData::fromJSON({{"title", ""}, {"completed", false}});
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::Error::JSONEmptyTitle);

    /* missing completed */
    result = TaskData::fromJSON({{"title", "title"}});
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::Error::JSONMissingRequiredField);
}
} // namespace test::mods
