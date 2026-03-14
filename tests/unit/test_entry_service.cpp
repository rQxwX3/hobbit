#include <entry_service.hpp>
#include <gtest/gtest.h>

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

    std::vector<hbt::mods::Occurrence> occurrences;

    auto SetUp() -> void override {
        storage = std::make_shared<FakeStorageEngine>();

        repo = std::make_unique<
            hbt::repo::json::MultiItemRepository<hbt::mods::Entry>>(storage);

        repoPtr = repo.get();

        service = std::make_unique<hbt::core::EntryService>(std::move(repo));

        occurrences = {{std::chrono::weekday{1}, hbt::mods::Daypart::AFTERNOON},
                       {std::chrono::weekday{2}, hbt::mods::Daypart::MORNING},
                       {std::chrono::weekday{3}, hbt::mods::Daypart::NIGHT}};
    }
};

TEST_F(EntryServiceTest, CreatesEntry) {
    auto id{service->createEntry("entry", occurrences)};
    auto result{repoPtr->load(id)};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTitle(), "entry");
}
} // namespace test::core
