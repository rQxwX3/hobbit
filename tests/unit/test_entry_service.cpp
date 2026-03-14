#include <entry_service.hpp>
#include <gtest/gtest.h>

#include <fake_storage_engine.hpp>
#include <json_repository.hpp>

namespace test::core {
using test::util::FakeStorageEngine;

class EntryServiceTest : public ::testing::Test {
  protected:
    std::unique_ptr<hbt::repo::json::MultiItemRepository<hbt::mods::Entry>>
        repo;

    std::shared_ptr<FakeStorageEngine> storage;

    auto SetUp() -> void override {
        storage = std::make_shared<FakeStorageEngine>();

        repo = std::make_unique<
            hbt::repo::json::MultiItemRepository<hbt::mods::Entry>>(storage);
    }
};
} // namespace test::core
