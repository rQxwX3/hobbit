#include <gtest/gtest.h>

#include <datetime.hpp>
#include <duration.hpp>
#include <interval.hpp>
#include <recurrence.hpp>
#include <task_data.hpp>
#include <task_series.hpp>

#include <chrono>

namespace test::mods {

using hbt::mods::TaskData;
using hbt::mods::TaskSeries;

using hbt::mods::Date;
using hbt::mods::DateTime;
using hbt::mods::Duration;
using hbt::mods::Interval;

using hbt::mods::util::Recurrence;

using year = std::chrono::year;
using month = std::chrono::month;
using day = std::chrono::day;

static auto makeTask(DateTime dt) -> TaskData {
    return TaskData{"test-task", dt};
}

static auto makeDailyRecurrence() -> Recurrence {
    return Recurrence{Interval(Duration::days(1))};
}

static auto makeTwoDayRecurrence() -> Recurrence {
    return Recurrence{Interval(Duration::days(2))};
}

TEST(TaskSeriesTest, StopBeforeStartThrows) {
    auto start{DateTime(Date(year(2025), month(1), day(10)))};
    auto stop{DateTime(Date(year(2025), month(1), day(5)))};

    auto task{makeTask(start)};
    auto recurrence{makeDailyRecurrence()};

    EXPECT_THROW(TaskSeries(task, recurrence, stop), std::invalid_argument);
}

TEST(TaskSeriesTest, GetStartReturnsCorrectValue) {
    auto start{DateTime(Date(year(2025), month(1), day(1)))};

    auto task{makeTask(start)};
    auto recurrence{makeDailyRecurrence()};

    auto series{TaskSeries(task, recurrence, std::nullopt)};

    EXPECT_EQ(series.getStart().getDate(), start.getDate());
}

TEST(TaskSeriesTest, GetStopReturnsCorrectValue) {
    auto start{DateTime(Date(year(2025), month(1), day(1)))};
    auto stop{DateTime(Date(year(2025), month(1), day(10)))};

    auto task{makeTask(start)};
    auto recurrence{makeDailyRecurrence()};

    auto series{TaskSeries(task, recurrence, stop)};

    EXPECT_EQ(series.getStop()->getDate(), stop.getDate());
}

TEST(TaskSeriesTest, IsForDateRespectsStartBoundary) {
    auto start{DateTime(Date(year(2025), month(1), day(5)))};

    auto task{makeTask(start)};
    auto recurrence{makeDailyRecurrence()};

    auto series{TaskSeries(task, recurrence, std::nullopt)};

    EXPECT_FALSE(series.isForDate(Date(year(2025), month(1), day(1))));
    EXPECT_TRUE(series.isForDate(Date(year(2025), month(1), day(5))));
}

TEST(TaskSeriesTest, IsForDateRespectsStopBoundary) {
    auto start{DateTime(Date(year(2025), month(1), day(1)))};
    auto stop{DateTime(Date(year(2025), month(1), day(10)))};

    auto task{makeTask(start)};
    auto recurrence{makeDailyRecurrence()};

    auto series{TaskSeries(task, recurrence, stop)};

    EXPECT_TRUE(series.isForDate(Date(year(2025), month(1), day(5))));
    EXPECT_FALSE(series.isForDate(Date(year(2025), month(1), day(11))));
}

TEST(TaskSeriesTest, IsForDateRespectsRecurrenceGap) {
    auto start{DateTime(Date(year(2025), month(1), day(1)))};

    auto task{makeTask(start)};
    auto recurrence{makeTwoDayRecurrence()};

    auto series{TaskSeries(task, recurrence, std::nullopt)};

    EXPECT_TRUE(series.isForDate(Date(year(2025), month(1), day(3))));
    EXPECT_FALSE(series.isForDate(Date(year(2025), month(1), day(2))));
}

TEST(TaskSeriesTest, GenerateSingularsReturnsTasksForDate) {
    auto start{DateTime(Date(year(2025), month(1), day(1)))};

    auto task{makeTask(start)};
    auto recurrence{makeDailyRecurrence()};

    auto series{TaskSeries(task, recurrence, std::nullopt)};

    auto result{
        series.generateSingularsForDate(Date(year(2025), month(1), day(3)))};

    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.front().getDateTime().getDate(),
              Date(year(2025), month(1), day(3)));
}

TEST(TaskSeriesTest, GenerateSingularsEmptyWhenNoMatch) {
    auto start{DateTime(Date(year(2025), month(1), day(1)))};

    auto task{makeTask(start)};
    auto recurrence{makeTwoDayRecurrence()};

    auto series{TaskSeries(task, recurrence, std::nullopt)};

    auto result{
        series.generateSingularsForDate(Date(year(2025), month(1), day(3)))};

    EXPECT_TRUE(result.empty());
}

TEST(TaskSeriesTest, SetRecurrenceUpdatesBehavior) {
    auto start{DateTime(Date(year(2025), month(1), day(1)))};

    auto task{makeTask(start)};
    auto recurrence1{makeDailyRecurrence()};
    auto recurrence2{makeTwoDayRecurrence()};

    auto series{TaskSeries(task, recurrence1, std::nullopt)};
    series.setRecurrence(recurrence2);

    EXPECT_TRUE(series.isForDate(Date(year(2025), month(1), day(3))));
    EXPECT_FALSE(series.isForDate(Date(year(2025), month(1), day(2))));
}

TEST(TaskSeriesTest, SetStopUpdatesValue) {
    auto start{DateTime(Date(year(2025), month(1), day(1)))};
    auto stop1{DateTime(Date(year(2025), month(1), day(10)))};
    auto stop2{DateTime(Date(year(2025), month(1), day(20)))};

    auto task{makeTask(start)};
    auto recurrence{makeDailyRecurrence()};

    auto series{TaskSeries(task, recurrence, stop1)};
    series.setStop(stop2);

    EXPECT_EQ(series.getStop()->getDate(), stop2.getDate());
}

TEST(TaskSeriesTest, FromJSONRoundTrip) {
    auto start{DateTime(Date(year(2025), month(1), day(1)))};
    auto stop{DateTime(Date(year(2025), month(1), day(10)))};

    auto task{makeTask(start)};
    auto recurrence{makeDailyRecurrence()};

    auto original{TaskSeries(task, recurrence, stop)};

    auto json{original.toJSON()};
    auto restored{TaskSeries::fromJSON(json)};

    ASSERT_TRUE(restored.has_value());

    EXPECT_EQ(restored->getStart().getDate(), original.getStart().getDate());

    EXPECT_EQ(restored->getStop()->getDate(), original.getStop()->getDate());
}

TEST(TaskSeriesTest, FromJSONMissingFieldFails) {
    auto json{nlohmann::json{{"task", {}}, {"recurrence", {}}}};

    auto result{TaskSeries::fromJSON(json)};

    EXPECT_FALSE(result.has_value());
}
} // namespace test::mods
