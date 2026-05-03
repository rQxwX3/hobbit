#include <gtest/gtest.h>

#include <datetime.hpp>
#include <deadline.hpp>
#include <duration.hpp>
#include <interval.hpp>

#include <chrono>

namespace test::mods {

using hbt::mods::Date;
using hbt::mods::DateTime;
using hbt::mods::Deadline;
using hbt::mods::Interval;

using year = std::chrono::year;
using month = std::chrono::month;
using day = std::chrono::day;

TEST(DeadlineTest, NullFactoryCreatesNull) {
    EXPECT_EQ(Deadline::null().getType(), Deadline::Type::Null);
}

TEST(DeadlineTest, IsNullReturnsTrueOnNull) {
    EXPECT_TRUE(Deadline::null().isNull());
}

TEST(DeadlineTest, IsNullReturnsFalseOnNotNull) {
    EXPECT_FALSE(Deadline(Interval(Duration::days(7))).isNull());

    EXPECT_FALSE(Deadline(DateTime::now()).isNull());
}

TEST(DeadlineTest, IntervalTypeIsAccepted) {
    auto deadline{Deadline(Interval(Duration::days(7)))};

    EXPECT_EQ(deadline.getType(), Deadline::Type::Interval);
}

TEST(DeadlineTest, DateTimeTypeIsAccepted) {
    auto deadline{Deadline(DateTime(Date(2025, 1, 1)))};

    EXPECT_EQ(deadline.getType(), Deadline::Type::DateTime);
}

TEST(DeadlineTest, NullTypeIsAccepted) {
    auto deadline{Deadline(std::monostate())};

    EXPECT_EQ(deadline.getType(), Deadline::Type::Null);
}

TEST(DeadlineTest, GetIntervalReturnsCorrectValue) {
    auto interval{Interval(Duration::days(10))};
    auto deadline{Deadline(interval)};

    EXPECT_EQ(deadline.getInterval().getDuration(), interval.getDuration());
}

TEST(DeadlineTest, GetDateTimeReturnsCorrectValue) {
    auto dt{DateTime(Date(2025, 2, 3))};
    auto deadline{Deadline(dt)};

    EXPECT_EQ(deadline.getDateTime().getDate(), dt.getDate());
}

TEST(DeadlineTest, IntervalToJSONHasCorrectShape) {
    auto deadline{Deadline(Interval(Duration::days(5)))};

    auto json{deadline.toJSON()};

    EXPECT_TRUE(json.contains("type"));
    EXPECT_TRUE(json.contains("interval"));
}

TEST(DeadlineTest, IntervalJSONRoundTrip) {
    auto original{Deadline(Interval(Duration::days(9)))};

    auto json{original.toJSON()};
    auto restored{Deadline::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getType(), Deadline::Type::Interval);
    EXPECT_EQ(restored->getInterval().getDuration(),
              original.getInterval().getDuration());
}

TEST(DeadlineTest, IntervalJSONMissingFieldFails) {
    auto json{nlohmann::json{{"type", "interval"}}};

    auto result{Deadline::fromJSON(json)};

    EXPECT_FALSE(result.has_value());
}

TEST(DeadlineTest, DateTimeToJSONHasCorrectShape) {
    auto dt{DateTime(Date(2025, 3, 10))};
    auto deadline{Deadline(dt)};

    auto json{deadline.toJSON()};

    EXPECT_TRUE(json.contains("type"));
    EXPECT_TRUE(json.contains("datetime"));
}

TEST(DeadlineTest, DateTimeJSONRoundTrip) {
    auto dt{DateTime(Date(2025, 4, 20))};
    auto original{Deadline(dt)};

    auto json{original.toJSON()};
    auto restored{Deadline::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getType(), Deadline::Type::DateTime);
    EXPECT_EQ(restored->getDateTime().getDate(),
              original.getDateTime().getDate());
}

TEST(DeadlineTest, DateTimeJSONMissingFieldFails) {
    auto json{nlohmann::json{{"type", "datetime"}}};

    auto result{Deadline::fromJSON(json)};

    EXPECT_FALSE(result.has_value());
}

TEST(DeadlineTest, MissingTypeFieldFails) {
    auto json{nlohmann::json{{"interval", {}}}};

    auto result{Deadline::fromJSON(json)};

    EXPECT_FALSE(result.has_value());
}

TEST(DeadlineTest, UnsupportedTypeFails) {
    auto json{nlohmann::json{{"type", "banana"}, {"interval", {}}}};

    auto result{Deadline::fromJSON(json)};

    EXPECT_FALSE(result.has_value());
}
} // namespace test::mods
