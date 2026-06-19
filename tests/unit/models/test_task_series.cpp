#include <gtest/gtest.h>

#include <datetime.hpp>
#include <interval.hpp>
#include <recurrence.hpp>
#include <task_data.hpp>
#include <task_series.hpp>

#include <chrono>

namespace test::mods {
using hbt::mods::TaskData;
using hbt::mods::TaskSeries;

using hbt::mods::DateTime;
using hbt::mods::Interval;

using hbt::mods::util::IntervalRecurrencePattern;
using hbt::mods::util::Recurrence;

using year = std::chrono::year;
using month = std::chrono::month;
using day = std::chrono::day;

static auto defaultTaskData{TaskData("TaskData")};
static auto dailyRecurrence(
    Recurrence(IntervalRecurrencePattern(DateTime::now(), Interval::days(1))));
static auto defaultTaskSeries{TaskSeries(defaultTaskData, dailyRecurrence)};

TEST(TaskSeriesTest, ThrowsOnStopBeforeStart) {
    auto startDateTime{DateTime::now() + Interval::days(1)};
    auto invalidEndDateTime{DateTime::now()};

    EXPECT_THROW(TaskSeries(TaskData("Task", startDateTime), dailyRecurrence,
                            invalidEndDateTime),
                 std::invalid_argument);

    EXPECT_THROW(TaskSeries(TaskData("Task", startDateTime), dailyRecurrence)
                     .setEndDateTime(invalidEndDateTime),
                 std::invalid_argument);
}

TEST(TaskSeriesTest, IsForDateRespectsStartBoundary) {
    auto startDateTime{DateTime({2025, 1, 5})};

    auto taskData{TaskData("Task", startDateTime)};
    auto taskSeries{TaskSeries(taskData, dailyRecurrence)};

    EXPECT_FALSE(taskSeries.happensOnDate(DateTime({2025, 1, 1}))); // too early
    EXPECT_TRUE(taskSeries.happensOnDate(DateTime({2025, 1, 6})));
}

TEST(TaskSeriesTest, IsForDateRespectsStopBoundary) {
    auto startDateTime{DateTime({2025, 1, 1})};
    auto stopDateTime{DateTime({2025, 1, 10})};

    auto taskData{TaskData("Task", startDateTime)};

    auto series{TaskSeries(taskData, dailyRecurrence, stopDateTime)};

    EXPECT_TRUE(series.happensOnDate(DateTime({2025, 1, 5})));
    EXPECT_FALSE(series.happensOnDate(DateTime({2025, 1, 11})));
}

// TEST(TaskSeriesTest, IsForDateRespectsRecurrenceGap) {
//     auto start{DateTime(Date(2025, 1, 1))};
//
//     auto task{makeTask(start)};
//     auto recurrence{makeTwoDayRecurrence()};
//
//     auto series{TaskSeries(task, recurrence, std::nullopt)};
//
//     EXPECT_TRUE(series.isForDate(Date(2025, 1, 3)));
//     EXPECT_FALSE(series.isForDate(Date(2025, 1, 2)));
// }
//
// TEST(TaskSeriesTest, GenerateSingularsReturnsTasksForDate) {
//     auto start{DateTime(Date(2025, 1, 1))};
//
//     auto task{makeTask(start)};
//     auto recurrence{makeDailyRecurrence()};
//
//     auto series{TaskSeries(task, recurrence, std::nullopt)};
//
//     auto result{series.generateSingularsForDate(Date(2025, 1, 3))};
//
//     EXPECT_FALSE(result.empty());
//     EXPECT_EQ(result.front().getDateTime().getDays(), Date(2025, 1, 3));
// }
//
// TEST(TaskSeriesTest, GenerateSingularsEmptyWhenNoMatch) {
//     auto start{DateTime(Date(2025, 1, 1))};
//
//     auto task{makeTask(start)};
//     auto recurrence{makeTwoDayRecurrence()};
//
//     auto series{TaskSeries(task, recurrence, std::nullopt)};
//
//     auto result{series.generateSingularsForDate(Date(2025, 1, 3))};
//
//     EXPECT_TRUE(result.empty());
// }
//
// TEST(TaskSeriesTest, SetRecurrenceUpdatesBehavior) {
//     auto start{DateTime(Date(2025, 1, 1))};
//
//     auto task{makeTask(start)};
//     auto recurrence1{makeDailyRecurrence()};
//     auto recurrence2{makeTwoDayRecurrence()};
//
//     auto series{TaskSeries(task, recurrence1, std::nullopt)};
//     series.setRecurrence(recurrence2);
//
//     EXPECT_TRUE(series.isForDate(Date(2025, 1, 3)));
//     EXPECT_FALSE(series.isForDate(Date(2025, 1, 2)));
// }
//
// TEST(TaskSeriesTest, SetStopUpdatesValue) {
//     auto start{DateTime(Date(2025, 1, 1))};
//     auto stop1{DateTime(Date(2025, 1, 10))};
//     auto stop2{DateTime(Date(2025, 1, 20))};
//
//     auto task{makeTask(start)};
//     auto recurrence{makeDailyRecurrence()};
//
//     auto series{TaskSeries(task, recurrence, stop1)};
//     series.setStop(stop2);
//
//     EXPECT_EQ(series.getStop()->getDays(), stop2.getDays());
// }
//
// TEST(TaskSeriesTest, FromJSONRoundTrip) {
//     auto start{DateTime(Date(2025, 1, 1))};
//     auto stop{DateTime(Date(2025, 1, 10))};
//
//     auto task{makeTask(start)};
//     auto recurrence{makeDailyRecurrence()};
//
//     auto original{TaskSeries(task, recurrence, stop)};
//
//     auto json{original.toJSON()};
//     auto restored{TaskSeries::fromJSON(json)};
//
//     ASSERT_TRUE(restored.has_value());
//
//     EXPECT_EQ(restored->getStart().getDays(), original.getStart().getDays());
//
//     EXPECT_EQ(restored->getStop()->getDays(), original.getStop()->getDays());
// }
//
// TEST(TaskSeriesTest, FromJSONMissingFieldFails) {
//     auto json{nlohmann::json{{"task", {}}, {"recurrence", {}}}};
//
//     auto result{TaskSeries::fromJSON(json)};
//
//     EXPECT_FALSE(result.has_value());
// }
} // namespace test::mods
