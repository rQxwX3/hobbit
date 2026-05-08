#include <gtest/gtest.h>

#include <datetime.hpp>
#include <deadline.hpp>
#include <interval.hpp>

namespace test::mods {
using hbt::mods::DateTime;
using hbt::mods::Deadline;
using hbt::mods::Interval;

TEST(DeadlineTest, IntervalTypeIsAccepted) {
    auto deadline{Deadline(Interval::days(7))};

    EXPECT_EQ(deadline.getType(), Deadline::Type::Interval);
}

TEST(DeadlineTest, DateTimeTypeIsAccepted) {
    auto deadline{Deadline(DateTime({2025, 1, 1}))};

    EXPECT_EQ(deadline.getType(), Deadline::Type::DateTime);
}

TEST(DeadlineTest, NullTypeIsAccepted) {
    auto deadline{Deadline(std::monostate())};

    EXPECT_EQ(deadline.getType(), Deadline::Type::Null);
}

TEST(DeadlineTest, NullFactoryCreatesNull) {
    EXPECT_EQ(Deadline::null().getType(), Deadline::Type::Null);
}

TEST(DeadlineTest, IsNullReturnsTrueOnNull) {
    EXPECT_TRUE(Deadline::null().isNull());
}

TEST(DeadlineTest, IsNullReturnsFalseOnNotNull) {
    EXPECT_FALSE(Deadline(Interval::days(7)).isNull());

    EXPECT_FALSE(Deadline(DateTime::now()).isNull());
}

TEST(DeadlineTest, GetIntervalReturnsCorrectValueOnIntervalDeadline) {
    auto interval{Interval::days(10)};
    auto deadline{Deadline(interval)};

    EXPECT_EQ(deadline.getInterval(), interval);
}

TEST(DeadlineTest, GetIntervalThrowsOnNonIntervalDeadline) {
    EXPECT_THROW(Deadline(DateTime()).getInterval(), std::runtime_error);

    EXPECT_THROW(Deadline::null().getInterval(), std::runtime_error);
}

TEST(DeadlineTest, GetDateTimeReturnsCorrectValueOnDateTimeDeadline) {
    auto dt{DateTime({2025, 2, 3})};
    auto deadline{Deadline(dt)};

    EXPECT_EQ(deadline.getDateTime(), dt);
}

TEST(DeadlineTest, GetDateTimeThrowsOnNonDateTimeDeadline) {
    EXPECT_THROW(Deadline(Interval()).getDateTime(), std::runtime_error);

    EXPECT_THROW(Deadline::null().getDateTime(), std::runtime_error);
}

TEST(DeadlineTest, IntervalToJSON) {
    auto interval{Interval::days(5)};
    auto deadline{Deadline(interval)};

    auto json = deadline.toJSON();

    ASSERT_TRUE(json.contains("type"));
    ASSERT_TRUE(json.contains("interval"));

    EXPECT_EQ(json["type"].get<std::string>(), "interval");

    ASSERT_TRUE(Interval::fromJSON(json["interval"]));
    EXPECT_EQ(Interval::fromJSON(json["interval"]).value(), interval);
}

TEST(DeadlineTest, IntervalJSONFailsOnMissingField) {
    auto json = nlohmann::json{{"type", "interval"}};

    /* missing interval field */
    EXPECT_FALSE(Deadline::fromJSON(json));

    /* empty json */
    EXPECT_FALSE(Deadline::fromJSON(nlohmann::json()));
}

TEST(DeadlineTest, IntervalJSONFailsOnTypeValueMismatch) {
    auto json = nlohmann::json{{"type", "interval"},
                               {"interval", DateTime::now().toISO8601String()}};

    EXPECT_FALSE(Deadline::fromJSON(json));
}

TEST(DeadlineTest, IntervalJSONFailsOnIncorrectType) {
    auto json = nlohmann::json{{"type", "datetime"},
                               {"interval", Interval::days(1).toJSON()}};

    EXPECT_FALSE(Deadline::fromJSON(json));
}

TEST(DeadlineTest, IntervalJSONRoundTrip) {
    auto interval{Interval::days(12)};
    auto original{Deadline(interval)};

    auto json = original.toJSON();
    auto restored{Deadline::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getType(), Deadline::Type::Interval);
    EXPECT_EQ(restored->getInterval(), interval);
}

TEST(DeadlineTest, DateTimeToJSON) {
    auto datetime{DateTime::now()};
    auto deadline{Deadline(datetime)};

    auto json = deadline.toJSON();

    ASSERT_TRUE(json.contains("type"));
    ASSERT_TRUE(json.contains("datetime"));

    EXPECT_EQ(json["type"].get<std::string>(), "datetime");

    auto jsonDateTime = json["datetime"].get<std::string>();
    ASSERT_TRUE(DateTime::fromISO8601String(jsonDateTime));
    EXPECT_EQ(DateTime::fromISO8601String(jsonDateTime).value(), datetime);
}

TEST(DeadlineTest, DatetimeJSONFailsOnMissingField) {
    auto json = nlohmann::json{{"type", "datetime"}};

    /* missing interval field */
    EXPECT_FALSE(Deadline::fromJSON(json));

    /* empty json */
    EXPECT_FALSE(Deadline::fromJSON(nlohmann::json()));
}

TEST(DeadlineTest, DateTimeJSONFailsOnIncorrectType) {
    auto json = nlohmann::json{{"type", "interval"},
                               {"datetime", DateTime::now().toISO8601String()}};

    EXPECT_FALSE(Deadline::fromJSON(json));
}

TEST(DeadlineTest, DeadlineJSONFailsOnTypeValueMismatch) {
    auto json = nlohmann::json{{"type", "datetime"},
                               {"interval", Interval::days(1).toJSON()}};

    EXPECT_FALSE(Deadline::fromJSON(json));
}

TEST(DeadlineTest, DateTimeJSONRoundTrip) {
    auto dt{DateTime({2025, 4, 20})};
    auto original{Deadline(dt)};

    auto json = original.toJSON();
    auto restored{Deadline::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getType(), Deadline::Type::DateTime);
    EXPECT_EQ(restored->getDateTime(), original.getDateTime());
}
} // namespace test::mods
