#include <gtest/gtest.h>

#include <singular_task.hpp>

namespace test::mods {
using hbt::mods::DateTime;
using hbt::mods::Deadline;
using hbt::mods::Interval;
using hbt::mods::SingularTask;
using hbt::mods::TaskData;

TEST(SingularTaskTest, IsForDate) {
    auto singularTask{SingularTask(TaskData("Title"))};
    EXPECT_TRUE(singularTask.isForDate(DateTime::now()));
    EXPECT_FALSE(singularTask.isForDate(DateTime({2000, 1, 1})));

    singularTask = SingularTask(TaskData("Title", DateTime({2000, 1, 1})));
    EXPECT_TRUE(singularTask.isForDate(DateTime({2000, 1, 1})));
    EXPECT_FALSE(singularTask.isForDate(DateTime::now()));
}

TEST(SingularTaskTest, JSONRoundTrip) {
    auto original{SingularTask(TaskData("Title"))};
    auto json = SingularTask::JSON::encode(original);
    auto restored{SingularTask::JSON::decode(json)};

    ASSERT_TRUE(restored);
    EXPECT_EQ(original, restored);
}

TEST(SingularTaskTest, FromJSONFailsOnInvalidJSON) {
    auto json{nlohmann::json{}};

    /* empty json */
    auto result{SingularTask::JSON::decode(json)};
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::JSON::Error::MissingRequiredField);

    /* missing TaskData */
    json = {{SingularTask::JSON::isCompletedField, false}};
    result = SingularTask::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::JSON::Error::MissingRequiredField);

    /* invalid TaskData */
    json = {{SingularTask::JSON::taskDataField, ""},
            {SingularTask::JSON::isCompletedField, false}};
    result = SingularTask::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), SingularTask::JSON::Error::FailedToParseTaskData);

    /* invalid isCompleted */
    json = {{SingularTask::JSON::taskDataField,
             TaskData::JSON::encode(TaskData("Title"))},
            {SingularTask::JSON::isCompletedField, ""}};
    result = SingularTask::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              SingularTask::JSON::Error::FailedToParseIsCompleted);
}
} // namespace test::mods
