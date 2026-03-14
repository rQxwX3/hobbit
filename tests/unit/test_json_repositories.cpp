#include <json_repository.hpp>
#include <storage_engine.hpp>
#include <user.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <fake_model.hpp>
#include <fake_storage_engine.hpp>

namespace test::repo::json {
using test::util::FakeModel;
using test::util::FakeStorageEngine;

class SingleItemRepositoryTest : public ::testing::Test {
  protected:
    std::shared_ptr<FakeStorageEngine> storage;
    std::unique_ptr<hbt::repo::json::SingleItemRepository<FakeModel>> repo;

    auto SetUp() -> void override {
        storage = std::make_shared<FakeStorageEngine>();
        repo =
            std::make_unique<hbt::repo::json::SingleItemRepository<FakeModel>>(
                storage);
    }
};

TEST_F(SingleItemRepositoryTest, SaveAndLoad) {
    repo->save(FakeModel{"test"});

    auto result{repo->load()};
    ASSERT_TRUE(result.has_value());
}

TEST_F(SingleItemRepositoryTest, Exists) {
    repo->save(FakeModel{"test"});

    EXPECT_EQ(repo->exists(), true);
}

TEST_F(SingleItemRepositoryTest, Remove) {
    repo->save(FakeModel{"test"});

    repo->remove();

    EXPECT_EQ(repo->exists(), false);
}

TEST_F(SingleItemRepositoryTest, GetCount) {
    EXPECT_EQ(repo->getCount(), 0);

    repo->save(FakeModel{"test"});

    EXPECT_EQ(repo->getCount(), 1);
}

TEST_F(SingleItemRepositoryTest, GetAll) {
    repo->save(FakeModel{"test"});

    EXPECT_EQ(repo->getAll()[0].field, "test");
}

TEST_F(SingleItemRepositoryTest, Clear) {
    repo->save(FakeModel{"test"});

    repo->clear();

    EXPECT_EQ(repo->getCount(), 0);
}

class MultiItemRepositoryTest : public ::testing::Test {
  protected:
    std::shared_ptr<FakeStorageEngine> storage;
    std::unique_ptr<hbt::repo::json::MultiItemRepository<FakeModel>> repo;

    auto SetUp() -> void override {
        storage = std::make_shared<FakeStorageEngine>();
        repo =
            std::make_unique<hbt::repo::json::MultiItemRepository<FakeModel>>(
                storage, "counter");
    }
};

TEST_F(MultiItemRepositoryTest, SaveAndLoad) {
    auto id1{repo->save(FakeModel{"test"})};
    auto result1{repo->load(id1)};
    ASSERT_TRUE(result1.has_value());

    auto id2{repo->save(FakeModel{"test"})};
    auto result2{repo->load(id2)};
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());
}

TEST_F(MultiItemRepositoryTest, Exists) {
    auto id{repo->save(FakeModel{"test"})};

    EXPECT_EQ(repo->exists(id), true);
}

TEST_F(MultiItemRepositoryTest, Remove) {
    auto id1{repo->save(FakeModel{"test"})};
    auto id2{repo->save(FakeModel{"test"})};

    repo->remove(id1);
    EXPECT_EQ(repo->exists(id1), false);
    EXPECT_EQ(repo->exists(id2), true);
}

TEST_F(MultiItemRepositoryTest, GenerateID) {
    auto id1{repo->save(FakeModel{"test"})};
    EXPECT_EQ(id1, 1);

    auto id2{repo->save(FakeModel{"test"})};
    EXPECT_EQ(id2, 2);

    repo->remove(id2);

    auto id3{repo->save(FakeModel{"test"})};
    EXPECT_EQ(id3, 3);
}

TEST_F(MultiItemRepositoryTest, GetCount) {
    EXPECT_EQ(repo->getCount(), 0);

    auto id1{repo->save(FakeModel{"test"})};
    EXPECT_EQ(repo->getCount(), 1);

    auto id2{repo->save(FakeModel{"test"})};
    EXPECT_EQ(repo->getCount(), 2);
}

TEST_F(MultiItemRepositoryTest, GetAll) {
    auto id1{repo->save(FakeModel{"test1"})};
    auto id2{repo->save(FakeModel{"test2"})};
    auto id3{repo->save(FakeModel{"test3"})};

    auto all{repo->getAll()};
    EXPECT_EQ(all.size(), 3);

    std::vector<std::string> fields;
    fields.reserve(all.size());

    for (const auto &item : all) {
        fields.push_back(item.field);
    }

    EXPECT_THAT(fields,
                testing::UnorderedElementsAre("test1", "test2", "test3"));
}

TEST_F(MultiItemRepositoryTest, Clear) {
    auto id1{repo->save(FakeModel{"test"})};
    auto id2{repo->save(FakeModel{"test"})};

    repo->clear();

    EXPECT_EQ(repo->getCount(), 0);
}
} // namespace test::repo::json
