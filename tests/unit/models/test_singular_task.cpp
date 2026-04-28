#include <gtest/gtest.h>

#include <singular_task.hpp>

namespace test::mods {

using hbt::mods::Date;
using hbt::mods::DateTime;
using hbt::mods::Deadline;
using hbt::mods::SingularTask;
using hbt::mods::TaskData;

TEST(SingularTaskTest, ConstructionAndGetters) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    TaskData data{"Test", dt, false, std::nullopt};

    SingularTask task{data};

    EXPECT_EQ(task.getTitle(), "Test");
    EXPECT_EQ(task.getDateTime(), dt);
    EXPECT_FALSE(task.isCompleted());
    EXPECT_FALSE(task.hasDeadline());
}

TEST(SingularTaskTest, Setters) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    TaskData data{"Test", dt, false, std::nullopt};

    SingularTask task{data};

    task.setTitle("Updated");
    EXPECT_EQ(task.getTitle(), "Updated");

    auto newDt{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};
    task.setDateTime(newDt);
    EXPECT_EQ(task.getDateTime(), newDt);

    task.setCompleted(true);
    EXPECT_TRUE(task.isCompleted());
}

TEST(SingularTaskTest, DeadlineHandling) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    auto deadlineDT{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};
    auto deadline{Deadline::fromDateTime(deadlineDT)};

    TaskData data{"Test", dt, false, deadline};
    SingularTask task{data};

    EXPECT_TRUE(task.hasDeadline());
    EXPECT_EQ(task.getDeadline()->getDateTime(), deadlineDT);
}

TEST(SingularTaskTest, IsForDate) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    TaskData data{"Test", dt, false, std::nullopt};

    SingularTask task{data};

    auto sameDate{dt.getDate()};
    EXPECT_TRUE(task.isForDate(sameDate));

    auto otherDate{Date::fromISO8601String("2025-01-02").value()};
    EXPECT_FALSE(task.isForDate(otherDate));
}

TEST(SingularTaskTest, ToFromJSON) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    auto deadlineDT{DateTime::fromISO8601String("2025-01-02T10:00:00").value()};
    auto deadline{Deadline::fromDateTime(deadlineDT)};

    TaskData data{"Test", dt, true, deadline};
    SingularTask original{data};

    auto json{original.toJSON()};
    auto restored{SingularTask::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getTitle(), original.getTitle());
    EXPECT_EQ(restored->getDateTime(), original.getDateTime());
    EXPECT_EQ(restored->isCompleted(), original.isCompleted());
    ASSERT_TRUE(restored->hasDeadline());
    EXPECT_EQ(restored->getDeadline()->getDateTime(),
              original.getDeadline()->getDateTime());
}

TEST(SingularTaskTest, ToFromJSONWithoutDeadline) {
    auto dt{DateTime::fromISO8601String("2025-01-01T10:00:00").value()};
    TaskData data{"Test", dt, false, std::nullopt};

    SingularTask original{data};

    auto json{original.toJSON()};
    auto restored{SingularTask::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());
    EXPECT_FALSE(restored->hasDeadline());
}

TEST(SingularTaskTest, FromJSONInvalid) {
    nlohmann::json json{};

    // missing required field
    EXPECT_FALSE(SingularTask::fromJSON(json).has_value());

    // invalid TaskData inside
    json = {{"task", {{"invalid", "data"}}}};
    EXPECT_FALSE(SingularTask::fromJSON(json).has_value());
}

} // namespace test::mods
