// #include <gmock/gmock.h>
// #include <gtest/gtest.h>
//
// #include <datetime.hpp>
// #include <fake_storage_engine.hpp>
// #include <json_repository.hpp>
// #include <task_service.hpp>
//
// #include <chrono>
// #include <vector>
//
// namespace test::core {
// using test::util::FakeStorageEngine;
//
// class TaskServiceTest : public ::testing::Test {
//   protected:
//     std::unique_ptr<hbt::core::TaskService> service;
//
//     hbt::repo::json::MultiItemRepository<hbt::mods::Task> *repoPtr;
//
//     std::unique_ptr<hbt::repo::json::MultiItemRepository<hbt::mods::Task>>
//     repo;
//
//     std::shared_ptr<FakeStorageEngine> storage;
//
//     std::vector<hbt::mods::Occurrence> occurrences1;
//     std::vector<hbt::mods::Occurrence> occurrences2;
//
//     auto SetUp() -> void override {
//         storage = std::make_shared<FakeStorageEngine>();
//
//         repo = std::make_unique<
//             hbt::repo::json::MultiItemRepository<hbt::mods::Task>>(storage);
//
//         repoPtr = repo.get();
//
//         service = std::make_unique<hbt::core::TaskService>(std::move(repo));
//
//         using namespace std::chrono;
//
//         occurrences1 = {{hbt::mods::DateTime{
//                             year_month_day{year{2026}, month{3}, day{16}}}},
//                         {hbt::mods::DateTime{
//                             year_month_day{year{2026}, month{3}, day{18}}}}};
//
//         occurrences2 = {{hbt::mods::DateTime{
//                             year_month_day{year{2026}, month{3}, day{17}}}},
//                         {hbt::mods::DateTime{
//                             year_month_day{year{2026}, month{3}, day{19}}}}};
//     }
// };
//
// TEST_F(TaskServiceTest, CreatesSingleTask) {
//     auto id{service->createTask("task", occurrences1)};
//     auto result{repoPtr->load(id)};
//
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_EQ(result->isCompleted(), false);
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
// }
//
// TEST_F(TaskServiceTest, CreatesSingleTaskFromCopy) {
//     auto task{hbt::mods::Task{"task", occurrences1}};
//
//     auto id{service->createTask(task)};
//     auto result{repoPtr->load(id)};
//
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_EQ(result->isCompleted(), false);
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
// }
//
// TEST_F(TaskServiceTest, CreatesMultipleTasks) {
//     auto id1{service->createTask("task1", occurrences1)};
//     auto id2{service->createTask("task2", occurrences2)};
//
//     auto result1{repoPtr->load(id1)};
//     auto result2{repoPtr->load(id2)};
//
//     ASSERT_TRUE(result1.has_value());
//     ASSERT_TRUE(result2.has_value());
//
//     EXPECT_EQ(result1->getTitle(), "task1");
//     EXPECT_EQ(result2->getTitle(), "task2");
//     EXPECT_EQ(result1->isCompleted(), false);
//     EXPECT_EQ(result2->isCompleted(), false);
//
//     EXPECT_THAT(
//         result1->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
//     EXPECT_THAT(
//         result2->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences2[0], occurrences2[1]));
// }
//
// TEST_F(TaskServiceTest, CreatesMultipleTasksFromCopy) {
//     auto task1{hbt::mods::Task{"task1", occurrences1}};
//     auto task2{hbt::mods::Task{"task2", occurrences2}};
//
//     auto id1{service->createTask(task1)};
//     auto id2{service->createTask(task2)};
//
//     auto result1{repoPtr->load(id1)};
//     auto result2{repoPtr->load(id2)};
//
//     ASSERT_TRUE(result1.has_value());
//     ASSERT_TRUE(result2.has_value());
//
//     EXPECT_EQ(result1->getTitle(), "task1");
//     EXPECT_EQ(result2->getTitle(), "task2");
//     EXPECT_EQ(result1->isCompleted(), false);
//     EXPECT_EQ(result2->isCompleted(), false);
//
//     EXPECT_THAT(
//         result1->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
//     EXPECT_THAT(
//         result2->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences2[0], occurrences2[1]));
// }
//
// TEST_F(TaskServiceTest, ReturnsCorrectCount) {
//     EXPECT_EQ(service->getCount(), 0);
//
//     service->createTask("task", occurrences1);
//     EXPECT_EQ(service->getCount(), 1);
//
//     service->createTask("task", occurrences1);
//     EXPECT_EQ(service->getCount(), 2);
//
//     service->createTask("task", occurrences1);
//     EXPECT_EQ(service->getCount(), 3);
// }
//
// TEST_F(TaskServiceTest, DeletesTask) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->deleteTask(id);
//     EXPECT_EQ(service->getCount(), 0);
// }
//
// TEST_F(TaskServiceTest, DeletesMultipleTasks) {
//     auto id1{service->createTask("task1", occurrences1)};
//     auto id2{service->createTask("task2", occurrences1)};
//
//     service->deleteTask(id1);
//     EXPECT_EQ(service->getCount(), 1);
//
//     service->deleteTask(id2);
//     EXPECT_EQ(service->getCount(), 0);
// }
//
// TEST_F(TaskServiceTest, DeletingNonExistentTaskDoesNothing) {
//     auto id{service->createTask("task", occurrences1)};
//     EXPECT_EQ(service->getCount(), 1);
//
//     service->deleteTask(id + 1);
//     EXPECT_EQ(service->getCount(), 1);
// }
//
// TEST_F(TaskServiceTest, ChangesTaskTitle) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->changeTaskTitle(id, "todo");
//
//     auto result{repoPtr->load(id)};
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "todo");
//     EXPECT_EQ(result->isCompleted(), false);
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
// }
//
// TEST_F(TaskServiceTest, ChangingTitleOfNonExistentTaskDoesNothing) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->changeTaskTitle(id + 1, "todo");
//
//     auto result{repoPtr->load(id)};
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_EQ(result->isCompleted(), false);
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
// }
//
// TEST_F(TaskServiceTest, ChangesTaskOccurrences) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->changeTaskOccurrences(id, occurrences2);
//
//     auto result{repoPtr->load(id)};
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_EQ(result->isCompleted(), false);
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences2[0], occurrences2[1]));
// }
//
// TEST_F(TaskServiceTest, ChangingOccurrencesOfNonExistentTaskDoesNothing) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->changeTaskOccurrences(id + 1, occurrences2);
//
//     auto result{repoPtr->load(id)};
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_EQ(result->isCompleted(), false);
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
// }
//
// TEST_F(TaskServiceTest, CompletesTask) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->completeTask(id);
//     auto result{repoPtr->load(id)};
//
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
//     EXPECT_EQ(result->isCompleted(), true);
// }
//
// TEST_F(TaskServiceTest, UncompletesTask) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->completeTask(id);
//     service->uncompleteTask(id);
//     auto result{repoPtr->load(id)};
//
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
//     EXPECT_EQ(result->isCompleted(), false);
// }
//
// TEST_F(TaskServiceTest, CompletingUncompletingNonExistentTaskDoesNothing) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->completeTask(id + 1);
//     service->uncompleteTask(id + 2);
//     auto result{repoPtr->load(id)};
//
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
//     EXPECT_EQ(result->isCompleted(), false);
// }
//
// TEST_F(TaskServiceTest, CompletingCompletedTaskDoesNothing) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->completeTask(id);
//     auto result{repoPtr->load(id)};
//
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
//     EXPECT_EQ(result->isCompleted(), true);
// }
//
// TEST_F(TaskServiceTest, UncompletingUncompletedTaskDoesNothing) {
//     auto id{service->createTask("task", occurrences1)};
//
//     service->uncompleteTask(id);
//     auto result{repoPtr->load(id)};
//
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getTitle(), "task");
//     EXPECT_THAT(
//         result->getOccurrences(),
//         testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
//     EXPECT_EQ(result->isCompleted(), false);
// }
//
// TEST_F(TaskServiceTest, GetsTasksForDate) {
//     auto id1{service->createTask("task1", occurrences1)};
//     auto id2{service->createTask("task2", occurrences2)};
//     auto id3{service->createTask("task3", occurrences1)};
//     auto id4{service->createTask("task4", occurrences2)};
//
//     auto date{hbt::mods::DateTime{occurrences1[0].getDateTime()}}; //
//     wednesday auto results{service->getTasksForDate(date)};
//
//     ASSERT_EQ(results.size(), 2);
//
//     auto resultTitles{std::vector<std::string>()};
//     resultTitles.push_back(results[0].getTitle());
//     resultTitles.push_back(results[1].getTitle());
//
//     EXPECT_THAT(resultTitles, testing::UnorderedElementsAre("task1",
//     "task3"));
// }
// } // namespace test::core
