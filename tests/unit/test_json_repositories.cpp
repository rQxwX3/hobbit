#include <json_repository.hpp>
#include <storage_engine.hpp>
#include <user.hpp>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <memory>
#include <string>
#include <unordered_map>

class FakeStorageEngine : public hbt::store::StorageEngine {
  private:
    std::unordered_map<std::string, std::string> data_;

  public:
    auto write(const std::string &key, const std::string &value)
        -> void override {
        data_[key] = value;
    }

    [[nodiscard]] auto read(const std::string &key) const
        -> std::optional<std::string> override {
        auto it{data_.find(key)};

        if (it == data_.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    auto remove(const std::string &key) -> void override { data_.erase(key); }

    [[nodiscard]] auto exists(const std::string &key) const -> bool override {
        return data_.contains(key);
    }

  public:
    [[nodiscard]] auto getCount() const -> size_t override {
        return data_.size();
    }

    [[nodiscard]] auto getAll() const -> std::vector<std::string> override {
        std::vector<std::string> result(data_.size());

        for (const auto &[key, value] : data_) {
            result.push_back(value);
        }

        return result;
    }

    [[nodiscard]] auto getKeys() const -> std::vector<std::string> override {
        std::vector<std::string> result(data_.size());

        for (const auto &[key, value] : data_) {
            result.push_back(key);
        }

        return result;
    }

    auto clear() -> void override { data_.clear(); }

  public:
    ~FakeStorageEngine() override = default;
};

class FakeModel {
  public:
    std::string field;

  public:
    FakeModel(std::string field) : field{std::move(field)} {}

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json {
        return {{"field", field}};
    }

    [[nodiscard]] static auto fromJSON(const nlohmann::json &j) -> FakeModel {
        return FakeModel{j["field"].get<std::string>()};
    }
};

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
