#include <gtest/gtest.h>

#include <singular_task.hpp>

namespace test::mods {
using hbt::mods::DateTime;
using hbt::mods::Deadline;
using hbt::mods::Interval;
using hbt::mods::SingularTask;
using hbt::mods::TaskData;

const auto taskData{TaskData("Title", false)};
const auto deadlineInPast{Deadline(DateTime({2000, 1, 1}))};
const auto deadlineYearFromNow{Deadline(DateTime::now() + Interval::years(1))};

TEST(SingularTaskTest, CtorThrowsOnInvalidDeadline) {
    EXPECT_THROW(SingularTask(taskData, DateTime::now(), deadlineInPast),
                 std::invalid_argument);
}

TEST(SingularTaskTest, SetterThrowsOnInvalidDeadline) {
    auto singularTask{
        SingularTask(taskData, DateTime::now(),
                     Deadline(DateTime::now() + Interval::days(1)))};

    EXPECT_THROW(singularTask.setDeadline(deadlineInPast),
                 std::invalid_argument);
}

TEST(SingularTaskTest, SetterThrowsOnInvalidDateTime) {
    auto singularTask{
        SingularTask(taskData, DateTime::now(), deadlineYearFromNow)};

    EXPECT_THROW(singularTask.setDateTime(deadlineYearFromNow.getDateTime() +
                                          Interval::years(1)),
                 std::invalid_argument);
}

TEST(SingularTaskTest, Getters) {
    auto singularTask{
        SingularTask(taskData, DateTime::now(), deadlineYearFromNow)};

    EXPECT_EQ(singularTask.getTitle(), taskData.getTitle());
    EXPECT_EQ(singularTask.getDateTime(), DateTime::now());
    EXPECT_EQ(singularTask.isCompleted(), taskData.isCompleted());
    EXPECT_EQ(singularTask.getDeadline(), deadlineYearFromNow);
}

TEST(SingularTaskTest, IsForDateReturnsTrueCorrectly) {
    auto singularTask{
        SingularTask(taskData, DateTime::now(), deadlineYearFromNow)};

    EXPECT_TRUE(singularTask.isForDate(DateTime::now()));

    singularTask =
        SingularTask(taskData, DateTime({2000, 1, 1}), deadlineYearFromNow);

    EXPECT_TRUE(singularTask.isForDate(DateTime({2000, 1, 1})));
}

TEST(SingularTaskTest, IsForDateReturnsFalseCorrectly) {
    auto singularTask{
        SingularTask(taskData, DateTime::now(), deadlineYearFromNow)};

    EXPECT_FALSE(singularTask.isForDate(DateTime({2000, 1, 1})));

    singularTask =
        SingularTask(taskData, DateTime({2000, 1, 1}), deadlineYearFromNow);

    EXPECT_FALSE(singularTask.isForDate(DateTime::now()));
}

TEST(SingularTaskTest, JSONRoundTrip) {
    auto original{SingularTask(taskData, DateTime::now(), deadlineYearFromNow)};
    auto restored{SingularTask::fromJSON(original.toJSON())};

    EXPECT_EQ(original, restored);
}

TEST(SingularTaskTest, FromJSONFailsOnInvalidJSON) {
    auto json{nlohmann::json{}};

    /* empty json */
    auto result{SingularTask::fromJSON(json)};
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::Error::JSONMissingRequiredField);

    /* missing datetime and deadline */
    json = {taskData.toJSON()};
    result = SingularTask::fromJSON(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::Error::JSONMissingRequiredField);

    /* missing deadline */
    json = {taskData.toJSON(), {"datetime", DateTime::now().toISO8601String()}};
    result = SingularTask::fromJSON(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::Error::JSONMissingRequiredField);

    /* invalid task data */
    json = {{"task", {{"title", ""}, {"completed", false}}},
            {"datetime", DateTime::now().toISO8601String()},
            {"deadline", deadlineYearFromNow.toJSON()}};
    result = SingularTask::fromJSON(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::Error::JSONFailedToParseTaskData);

    /* invalid datetime */
    json = {{"task", taskData.toJSON()},
            {"datetime", "invalid"},
            {"deadline", deadlineYearFromNow.toJSON()}};
    result = SingularTask::fromJSON(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::Error::JSONFailedToParseDateTime);

    /* invalid deadline */
    json = {{"task", taskData.toJSON()},
            {"datetime", DateTime::now().toISO8601String()},
            {"deadline", "invalid"}};
    result = SingularTask::fromJSON(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::Error::JSONFailedToParseDeadline);

    /* missing deadline */
    json = {{"task", taskData.toJSON()},
            {"deadline", deadlineYearFromNow.toJSON()}};
    result = SingularTask::fromJSON(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::Error::JSONMissingRequiredField);

    /* invalid deadline (earlier than datetime) */
    json = {
        {"task", taskData.toJSON()},
        {"datetime", (deadlineYearFromNow.getDateTime() + Interval::years(1))
                         .toISO8601String()},
        {"deadline", deadlineYearFromNow.toJSON()}};
    result = SingularTask::fromJSON(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::Error::JSONInvalidDateTimeDeadline);
}
} // namespace test::mods
