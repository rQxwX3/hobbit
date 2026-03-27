#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <date.hpp>
#include <entry_service.hpp>
#include <fake_storage_engine.hpp>
#include <json_repository.hpp>

#include <chrono>
#include <vector>

namespace test::core {
using test::util::FakeStorageEngine;

class EntryServiceTest : public ::testing::Test {
  protected:
    std::unique_ptr<hbt::core::EntryService> service;

    hbt::repo::json::MultiItemRepository<hbt::mods::Entry> *repoPtr;

    std::unique_ptr<hbt::repo::json::MultiItemRepository<hbt::mods::Entry>>
        repo;

    std::shared_ptr<FakeStorageEngine> storage;

    std::vector<hbt::mods::Occurrence> occurrences1;
    std::vector<hbt::mods::Occurrence> occurrences2;

    auto SetUp() -> void override {
        storage = std::make_shared<FakeStorageEngine>();

        repo = std::make_unique<
            hbt::repo::json::MultiItemRepository<hbt::mods::Entry>>(storage);

        repoPtr = repo.get();

        service = std::make_unique<hbt::core::EntryService>(std::move(repo));

        using namespace std::chrono;

        occurrences1 = {
            {hbt::mods::Date{year_month_day{year{2026}, month{3}, day{16}}}},
            {hbt::mods::Date{year_month_day{year{2026}, month{3}, day{18}}}}};

        occurrences2 = {
            {hbt::mods::Date{year_month_day{year{2026}, month{3}, day{17}}}},
            {hbt::mods::Date{year_month_day{year{2026}, month{3}, day{19}}}}};
    }
};

TEST_F(EntryServiceTest, CreatesSingleEntry) {
    auto id{service->createEntry("entry", occurrences1)};
    auto result{repoPtr->load(id)};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_EQ(result->isCompleted(), false);
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
}

TEST_F(EntryServiceTest, CreatesSingleEntryFromCopy) {
    auto entry{hbt::mods::Entry{"entry", occurrences1}};

    auto id{service->createEntry(entry)};
    auto result{repoPtr->load(id)};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_EQ(result->isCompleted(), false);
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
}

TEST_F(EntryServiceTest, CreatesMultipleEntries) {
    auto id1{service->createEntry("entry1", occurrences1)};
    auto id2{service->createEntry("entry2", occurrences2)};

    auto result1{repoPtr->load(id1)};
    auto result2{repoPtr->load(id2)};

    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_EQ(result1->getTitle(), "entry1");
    EXPECT_EQ(result2->getTitle(), "entry2");
    EXPECT_EQ(result1->isCompleted(), false);
    EXPECT_EQ(result2->isCompleted(), false);

    EXPECT_THAT(
        result1->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
    EXPECT_THAT(
        result2->getOccurrences(),
        testing::UnorderedElementsAre(occurrences2[0], occurrences2[1]));
}

TEST_F(EntryServiceTest, CreatesMultipleEntriesFromCopy) {
    auto entry1{hbt::mods::Entry{"entry1", occurrences1}};
    auto entry2{hbt::mods::Entry{"entry2", occurrences2}};

    auto id1{service->createEntry(entry1)};
    auto id2{service->createEntry(entry2)};

    auto result1{repoPtr->load(id1)};
    auto result2{repoPtr->load(id2)};

    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_EQ(result1->getTitle(), "entry1");
    EXPECT_EQ(result2->getTitle(), "entry2");
    EXPECT_EQ(result1->isCompleted(), false);
    EXPECT_EQ(result2->isCompleted(), false);

    EXPECT_THAT(
        result1->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
    EXPECT_THAT(
        result2->getOccurrences(),
        testing::UnorderedElementsAre(occurrences2[0], occurrences2[1]));
}

TEST_F(EntryServiceTest, ReturnsCorrectCount) {
    EXPECT_EQ(service->getCount(), 0);

    service->createEntry("entry", occurrences1);
    EXPECT_EQ(service->getCount(), 1);

    service->createEntry("entry", occurrences1);
    EXPECT_EQ(service->getCount(), 2);

    service->createEntry("entry", occurrences1);
    EXPECT_EQ(service->getCount(), 3);
}

TEST_F(EntryServiceTest, DeletesEntry) {
    auto id{service->createEntry("entry", occurrences1)};

    service->deleteEntry(id);
    EXPECT_EQ(service->getCount(), 0);
}

TEST_F(EntryServiceTest, DeletesMultipleEntries) {
    auto id1{service->createEntry("entry1", occurrences1)};
    auto id2{service->createEntry("entry2", occurrences1)};

    service->deleteEntry(id1);
    EXPECT_EQ(service->getCount(), 1);

    service->deleteEntry(id2);
    EXPECT_EQ(service->getCount(), 0);
}

TEST_F(EntryServiceTest, DeletingNonExistentEntryDoesNothing) {
    auto id{service->createEntry("entry", occurrences1)};
    EXPECT_EQ(service->getCount(), 1);

    service->deleteEntry(id + 1);
    EXPECT_EQ(service->getCount(), 1);
}

TEST_F(EntryServiceTest, ChangesEntryTitle) {
    auto id{service->createEntry("entry", occurrences1)};

    service->changeEntryTitle(id, "todo");

    auto result{repoPtr->load(id)};
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "todo");
    EXPECT_EQ(result->isCompleted(), false);
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
}

TEST_F(EntryServiceTest, ChangingTitleOfNonExistentEntryDoesNothing) {
    auto id{service->createEntry("entry", occurrences1)};

    service->changeEntryTitle(id + 1, "todo");

    auto result{repoPtr->load(id)};
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_EQ(result->isCompleted(), false);
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
}

TEST_F(EntryServiceTest, ChangesEntryOccurrences) {
    auto id{service->createEntry("entry", occurrences1)};

    service->changeEntryOccurrences(id, occurrences2);

    auto result{repoPtr->load(id)};
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_EQ(result->isCompleted(), false);
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences2[0], occurrences2[1]));
}

TEST_F(EntryServiceTest, ChangingOccurrencesOfNonExistentEntryDoesNothing) {
    auto id{service->createEntry("entry", occurrences1)};

    service->changeEntryOccurrences(id + 1, occurrences2);

    auto result{repoPtr->load(id)};
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_EQ(result->isCompleted(), false);
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
}

TEST_F(EntryServiceTest, CompletesEntry) {
    auto id{service->createEntry("entry", occurrences1)};

    service->completeEntry(id);
    auto result{repoPtr->load(id)};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
    EXPECT_EQ(result->isCompleted(), true);
}

TEST_F(EntryServiceTest, UncompletesEntry) {
    auto id{service->createEntry("entry", occurrences1)};

    service->completeEntry(id);
    service->uncompleteEntry(id);
    auto result{repoPtr->load(id)};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
    EXPECT_EQ(result->isCompleted(), false);
}

TEST_F(EntryServiceTest, CompletingUncompletingNonExistentEntryDoesNothing) {
    auto id{service->createEntry("entry", occurrences1)};

    service->completeEntry(id + 1);
    service->uncompleteEntry(id + 2);
    auto result{repoPtr->load(id)};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
    EXPECT_EQ(result->isCompleted(), false);
}

TEST_F(EntryServiceTest, CompletingCompletedEntryDoesNothing) {
    auto id{service->createEntry("entry", occurrences1)};

    service->completeEntry(id);
    auto result{repoPtr->load(id)};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
    EXPECT_EQ(result->isCompleted(), true);
}

TEST_F(EntryServiceTest, UncompletingUncompletedEntryDoesNothing) {
    auto id{service->createEntry("entry", occurrences1)};

    service->uncompleteEntry(id);
    auto result{repoPtr->load(id)};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
    EXPECT_THAT(
        result->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
    EXPECT_EQ(result->isCompleted(), false);
}

TEST_F(EntryServiceTest, GetsEntriesForDate) {
    auto id1{service->createEntry("entry1", occurrences1)};
    auto id2{service->createEntry("entry2", occurrences2)};
    auto id3{service->createEntry("entry3", occurrences1)};
    auto id4{service->createEntry("entry4", occurrences2)};

    auto date{hbt::mods::Date{occurrences1[0].getDate()}}; // wednesday
    auto results{service->getEntriesForDate(date)};

    ASSERT_EQ(results.size(), 2);

    auto resultTitles{std::vector<std::string>()};
    resultTitles.push_back(results[0].getTitle());
    resultTitles.push_back(results[1].getTitle());

    EXPECT_THAT(resultTitles,
                testing::UnorderedElementsAre("entry1", "entry3"));
}
} // namespace test::core
