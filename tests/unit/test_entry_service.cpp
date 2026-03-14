#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <entry_service.hpp>
#include <fake_storage_engine.hpp>
#include <json_repository.hpp>

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

        occurrences1 = {
            {std::chrono::weekday{1}, hbt::mods::Daypart::AFTERNOON},
            {std::chrono::weekday{2}, hbt::mods::Daypart::MORNING}};

        occurrences2 = {{std::chrono::weekday{3}, hbt::mods::Daypart::NIGHT},
                        {std::chrono::weekday{4}, hbt::mods::Daypart::EVENING}};
    }
};

TEST_F(EntryServiceTest, CreatesSingleEntry) {
    auto id{service->createEntry("entry", occurrences1)};
    auto result{repoPtr->load(id)};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
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

    EXPECT_THAT(
        result1->getOccurrences(),
        testing::UnorderedElementsAre(occurrences1[0], occurrences1[1]));
    EXPECT_THAT(
        result2->getOccurrences(),
        testing::UnorderedElementsAre(occurrences2[0], occurrences2[1]));
}

// TEST_F(EntryServiceTest, ) {
//     auto id{service->createEntry("entry", occurrences1)};
//     auto result{repoPtr->load(id)};
// }
} // namespace test::core
