#include <gtest/gtest.h>

#include <task_data.hpp>

namespace test::mods {

using hbt::mods::DateTime;
using hbt::mods::Deadline;
using hbt::mods::TaskData;

TEST(TaskDataTest, ConstructionAndGetters) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};

    TaskData task{"Test", dt, false, std::nullopt};

    EXPECT_EQ(task.getTitle(), "Test");
    EXPECT_EQ(task.getDateTime(), dt);
    EXPECT_FALSE(task.isCompleted());
    EXPECT_FALSE(task.getDeadline().has_value());
}

TEST(TaskDataTest, Setters) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    TaskData task{"Test", dt, false, std::nullopt};

    task.setTitle("Updated");
    EXPECT_EQ(task.getTitle(), "Updated");

    auto newDt{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};
    task.setDateTime(newDt);
    EXPECT_EQ(task.getDateTime(), newDt);

    task.setCompleted(true);
    EXPECT_TRUE(task.isCompleted());
}

TEST(TaskDataTest, ValidDeadlineAfterDateTime) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    auto deadlineDT{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};

    auto deadline{Deadline(deadlineDT)};

    TaskData task{"Test", dt, false, deadline};

    ASSERT_TRUE(task.getDeadline().has_value());
    EXPECT_EQ(task.getDeadline()->getDateTime(), deadlineDT);
}

TEST(TaskDataTest, RejectsDeadlineBeforeDateTime) {
    auto dt{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};
    auto deadlineDT{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};

    auto deadline{Deadline(deadlineDT)};

    EXPECT_THROW(TaskData("Test", dt, false, deadline), std::invalid_argument);
}

TEST(TaskDataTest, RejectsDateTimeAfterDeadline) {
    auto dt{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};
    auto deadlineDT{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};

    auto deadline{Deadline(deadlineDT)};

    EXPECT_THROW(TaskData("Test", dt, false, deadline), std::invalid_argument);
}

TEST(TaskDataTest, SetDateTimeValidation) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    auto deadlineDT{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};
    auto deadline{Deadline(deadlineDT)};

    TaskData task{"Test", dt, false, deadline};

    auto invalidDT{DateTime::fromISO8601String("2025-01-03T10:00:00").value()};

    EXPECT_THROW(task.setDateTime(invalidDT), std::invalid_argument);
}

TEST(TaskDataTest, SetDeadlineValidation) {
    auto dt{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};
    TaskData task{"Test", dt, false, std::nullopt};

    auto invalidDeadlineDT{
        DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    auto deadline{Deadline(invalidDeadlineDT)};

    EXPECT_THROW(task.setDeadline(deadline), std::invalid_argument);
}

TEST(TaskDataTest, ToFromJSON) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    auto deadlineDT{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};
    auto deadline{Deadline(deadlineDT)};

    TaskData original{"Test", dt, true, deadline};

    auto json{original.toJSON()};
    auto restored{TaskData::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getTitle(), original.getTitle());
    EXPECT_EQ(restored->getDateTime(), original.getDateTime());
    EXPECT_EQ(restored->isCompleted(), original.isCompleted());
    ASSERT_TRUE(restored->getDeadline().has_value());
    EXPECT_EQ(restored->getDeadline()->getDateTime(),
              original.getDeadline()->getDateTime());
}

TEST(TaskDataTest, ToFromJSONWithNullDeadline) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};

    TaskData original{"Test", dt, false, std::nullopt};

    auto json{original.toJSON()};
    auto restored{TaskData::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());
    EXPECT_FALSE(restored->getDeadline().has_value());
}

TEST(TaskDataTest, FromJSONInvalid) {
    nlohmann::json json{};

    // missing fields
    EXPECT_FALSE(TaskData::fromJSON(json).has_value());

    // invalid datetime
    json = {{"title", "Test"},
            {"datetime", "invalid"},
            {"deadline", nullptr},
            {"completed", false}};
    EXPECT_FALSE(TaskData::fromJSON(json).has_value());
}

} // namespace test::mods
