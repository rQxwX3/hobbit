#include <gtest/gtest.h>

#include <task_data.hpp>

namespace test::mods {
using hbt::mods::DateTime;
using hbt::mods::Deadline;
using hbt::mods::Interval;
using hbt::mods::TaskData;

TEST(TaskDataTest, ThrowsOnEmptyTitle) {
    EXPECT_THROW(TaskData(""), std::invalid_argument);

    EXPECT_THROW(TaskData("Title").setTitle(""), std::invalid_argument);
}

TEST(TaskDataTest, ThrowsOnInvalidDeadline) {
    /* ctor datetime = deadline */
    EXPECT_THROW(TaskData("Title", DateTime::now(), Deadline(DateTime::now())),
                 std::invalid_argument);

    /* ctor datetime > deadline */
    EXPECT_THROW(TaskData("Title", DateTime::now() + Interval::days(1),
                          Deadline(DateTime::now())),
                 std::invalid_argument);

    /* setter datetime = deadline */
    EXPECT_THROW(TaskData("Title", DateTime::now())
                     .setDeadline(Deadline(DateTime::now())),
                 std::invalid_argument);

    /* setter datetime > deadline */
    EXPECT_THROW(TaskData("Title", DateTime::now() + Interval::days(1))
                     .setDeadline(Deadline(DateTime::now())),
                 std::invalid_argument);
}

TEST(TaskDataTest, ThrowsOnInvalidDateTime) {
    EXPECT_THROW(TaskData("Title", DateTime::now(),
                          Deadline(DateTime::now() + Interval::days(1)))
                     .setDateTime(DateTime::now() + Interval::days(2)),
                 std::invalid_argument);
}

TEST(TaskDataTest, JSONRoundTrip) {
    auto original{TaskData("Title")};

    auto json = TaskData::JSON::encode(original);
    auto restored{TaskData::JSON::decode(json)};

    ASSERT_TRUE(restored);
    EXPECT_EQ(restored.value(), original);
}

TEST(TaskDataTest, FromJSONFailsOnInvalidJSON) {
    auto json = nlohmann::json();

    /* empty json */
    auto result{TaskData::JSON::decode(json)};
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::JSON::Error::MissingRequiredField);

    /* missing title */
    json = {{TaskData::JSON::dateTimeField, DateTime::now().toISO8601String()},
            {TaskData::JSON::deadlineField, Deadline::null().toJSON()}};
    result = TaskData::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::JSON::Error::MissingRequiredField);

    /* invalid title */
    json = {{TaskData::JSON::titleField, ""},
            {TaskData::JSON::dateTimeField, DateTime::now().toISO8601String()},
            {TaskData::JSON::deadlineField, Deadline::null().toJSON()}};
    result = TaskData::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::JSON::Error::FailedToValidateTitle);

    /* missing datetime */
    json = {
        {TaskData::JSON::titleField, "Title"},
        {TaskData::JSON::deadlineField, Deadline::null().toJSON()},
    };
    result = TaskData::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::JSON::Error::MissingRequiredField);

    /* invalid datetime */
    json = {{TaskData::JSON::titleField, "Title"},
            {TaskData::JSON::dateTimeField, ""},
            {TaskData::JSON::deadlineField, Deadline::null().toJSON()}};
    result = TaskData::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::JSON::Error::FailedToParseDateTime);

    /* missing deadline */
    json = {{TaskData::JSON::titleField, "Title"},
            {TaskData::JSON::dateTimeField, DateTime::now().toISO8601String()}};
    result = TaskData::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::JSON::Error::MissingRequiredField);

    /* missing deadline (empty )*/
    json = {{TaskData::JSON::titleField, "Title"},
            {TaskData::JSON::dateTimeField, DateTime::now().toISO8601String()},
            {TaskData::JSON::deadlineField, ""}};
    result = TaskData::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(), TaskData::JSON::Error::FailedToParseDeadline);

    /* invalid deadline (before datetime) */
    json = {
        {TaskData::JSON::titleField, "Title"},
        {TaskData::JSON::dateTimeField,
         (DateTime::now() + Interval::days(1)).toISO8601String()},
        {TaskData::JSON::deadlineField, Deadline(DateTime::now()).toJSON()}};
    result = TaskData::JSON::decode(json);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error(),
              TaskData::JSON::Error::FailedToValidateDeadlineAgainsDateTime);
}
} // namespace test::mods
