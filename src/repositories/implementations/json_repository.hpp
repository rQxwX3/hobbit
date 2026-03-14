#pragma once

#include <nlohmann/json.hpp>

#include <repository.hpp>
#include <storage_engine.hpp>

#include <concepts>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>

namespace hbt::repo::json {
template <typename T>
concept JSONSerializable =
    requires(const T &const_t, T &t, T &&rvalue_t, nlohmann::json j) {
        { const_t.toJSON() } -> std::convertible_to<nlohmann::json>;

        { std::move(rvalue_t).toJSON() } -> std::convertible_to<nlohmann::json>;

        { T::fromJSON(j) } -> std::same_as<T>;
    };

template <JSONSerializable T> class Repository {
  public:
    std::shared_ptr<hbt::store::StorageEngine> storage_;

  public:
    Repository(std::shared_ptr<hbt::store::StorageEngine> storage)
        : storage_{std::move(storage)} {}

    virtual ~Repository() = default;

  public:
    Repository(const Repository &) = delete;
    Repository(Repository &&) = delete;

    auto operator=(const Repository &) -> Repository & = delete;
    auto operator=(Repository &&) -> Repository & = delete;

  public:
    [[nodiscard]] auto serialize(const T &data) const -> std::string {
        return data.toJSON().dump();
    }

    [[nodiscard]] auto serialize(T &&data) const -> std::string {
        return std::move(data).toJSON().dump();
    }

    [[nodiscard]] auto deserialize(const std::string &raw) const
        -> std::optional<T> {
        if (raw.empty()) {
            return std::nullopt;
        }

        return std::optional<T>(T::fromJSON(nlohmann::json::parse(raw)));
    }

  public:
    [[nodiscard]] auto getAll() const -> std::vector<T> {
        std::vector<T> result;

        for (const auto &[key, value] : storage_->getKeyValuePairs()) {
            if (auto obj{deserialize(value)}) {
                result.push_back(obj);
            }
        }

        return result;
    }

    [[nodiscard]] auto getCount() const -> size_t {
        return storage_->getCount();
    }

    auto clear() -> void { storage_->clear(); }
};

template <JSONSerializable T>
class SingleItemRepository : public hbt::repo::SingleItemRepository<T> {
  private:
    hbt::repo::json::Repository<T> base;

  public:
    explicit SingleItemRepository(
        std::shared_ptr<hbt::store::StorageEngine> storage)
        : base{std::move(storage)} {}

  public:
    auto save(const T &data) -> void override {
        base.storage_->write("1", base.serialize(data));
    }

    auto save(T &&data) -> void override {
        base.storage_->write("1", base.serialize(std::move(data)));
    }

    auto update(const T &data) -> void override { save(data); }

    [[nodiscard]] auto load() const -> std::optional<T> override {
        auto value{base.storage_->read("1")};

        if (!value) {
            return std::nullopt;
        }

        return base.deserialize(*value);
    }

    auto remove() -> void override { base.storage_->remove("1"); }

    [[nodiscard]] auto exists() const -> bool override {
        return base.storage_->exists("1");
    }

  public:
    [[nodiscard]] auto getAll() const -> std::vector<T> override {
        std::vector<T> result;

        for (const auto &item : base.storage_->getKeyValuePairs()) {
            if (auto obj{base.deserialize(item.second)}) {
                result.push_back(obj.value());
            }
        }

        return result;
    }

    [[nodiscard]] auto getCount() const -> size_t override {
        return base.storage_->getCount();
    }

    auto clear() -> void override { base.storage_->clear(); }
};

template <typename TID>
concept IDCompatible = std::same_as<TID, std::size_t>;

template <JSONSerializable T, IDCompatible TID = std::size_t>
class MultiItemRepository : public hbt::repo::MultiItemRepository<T, TID> {
  private:
    hbt::repo::json::Repository<T> base_;
    std::string counterKey_;

  private:
    [[nodiscard]] auto generateID() -> TID {
        auto counter{base_.storage_->read(counterKey_)};
        auto nextID{counter ? std::stoull(counter.value()) + 1 : 1};

        base_.storage_->write(counterKey_, std::to_string(nextID));

        return nextID;
    }

  public:
    explicit MultiItemRepository(
        std::shared_ptr<hbt::store::StorageEngine> storage,
        std::string counterKey = "counter")
        : base_{std::move(storage)}, counterKey_{std::move(counterKey)} {}

  public:
    auto save(const T &data) -> TID override {
        auto id{generateID()};

        base_.storage_->write(std::to_string(id), base_.serialize(data));

        return id;
    }

    auto save(T &&data) -> TID override {
        auto id{generateID()};

        base_.storage_->write(std::to_string(id),
                              base_.serialize(std::move(data)));

        return id;
    }

    auto update(const TID &id, const T &data) -> void override {
        if (!exists(id)) {
            return;
        }

        base_.storage_->write(std::to_string(id), base_.serialize(data));
    }

    [[nodiscard]] auto load(const TID &id) const -> std::optional<T> override {
        auto value{base_.storage_->read(std::to_string(id))};

        if (!value) {
            return std::nullopt;
        }

        return base_.deserialize(*value);
    }

    auto remove(const TID &id) -> void override {
        base_.storage_->remove(std::to_string(id));
    }

    [[nodiscard]] auto exists(const TID &id) const -> bool override {
        return base_.storage_->exists(std::to_string(id));
    }

  public:
    [[nodiscard]] auto getAll() const -> std::vector<T> override {
        std::vector<T> result;

        for (const auto &item : base_.storage_->getKeyValuePairs()) {
            if (item.first == counterKey_) {
                continue;
            }

            if (auto obj{base_.deserialize(item.second)}) {
                result.push_back(obj.value());
            }
        }

        return result;
    }

    [[nodiscard]] auto getCount() const -> size_t override {
        auto storageCountIncludingCountKey{base_.storage_->getCount()};

        return storageCountIncludingCountKey > 0
                   ? storageCountIncludingCountKey - 1
                   : 0;
    }

    auto clear() -> void override { base_.storage_->clear(); }
};
} // namespace hbt::repo::json
