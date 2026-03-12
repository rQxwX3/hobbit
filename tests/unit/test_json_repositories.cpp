#include <storage_engine.hpp>

#include <gtest/gtest.h>

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
