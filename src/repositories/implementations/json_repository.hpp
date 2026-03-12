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
concept JSONSerializable = requires(T t, nlohmann::json j) {
    { t.toJSON() } -> std::same_as<nlohmann::json>;
    { T::fromJSON(j) } -> std::same_as<T>;
};

template <JSONSerializable T> class Repository {
  public:
    std::shared_ptr<hbt::store::StorageEngine> storage_;

  public:
    Repository(std::shared_ptr<hbt::store::StorageEngine> storage)
        : storage_{std::move(storage)} {}

  public:
    [[nodiscard]] auto serialize(const T &data) const -> std::string {
        return data.toJSON().dump();
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

        for (const auto &item : storage_->getAll()) {
            if (auto obj{deserialize(item)}) {
                result.push_back(obj);
            }
        }

        return result;
    }

    [[nodiscard]] auto getCount() const -> size_t {
        return storage_->getCount();
    }

    [[nodiscard]] auto getKeys() const -> std::vector<std::string> {
        return storage_->getKeys();
    }

    auto clear() -> void { storage_->clear(); }

  public:
    virtual ~Repository() = default;
};

template <JSONSerializable T>
class SingleItemRepository : public hbt::repo::SingleItemRepository<T> {
  private:
    hbt::repo::json::Repository<T> base;

  public:
    SingleItemRepository(std::shared_ptr<hbt::store::StorageEngine> storage)
        : base{std::move(storage)} {}

  public:
    auto save(const T &data) -> void override {
        base.storage_->write("1", base.serialize(data));
    }

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

        for (const auto &item : base.storage_->getAll()) {
            if (auto obj{base.deserialize(item)}) {
                result.push_back(obj.value());
            }
        }

        return result;
    }

    [[nodiscard]] auto getCount() const -> size_t override {
        return base.storage_->getCount();
    }

    [[nodiscard]] auto getKeys() const -> std::vector<std::string> override {
        return base.storage_->getKeys();
    }

    auto clear() -> void override { base.storage_->clear(); }
};

template <typename TID>
concept IDCompatible = std::same_as<TID, std::size_t>;

template <JSONSerializable T, IDCompatible TID = std::size_t>
class MultiItemRepository : public hbt::repo::MultiItemRepository<T, TID> {
  private:
    constexpr static std::string counterKey{"counter"};

  private:
    hbt::repo::json::Repository<T> base;

  private:
    [[nodiscard]] auto generateID() -> TID {
        auto counter{base.storage_->read(counterKey)};
        auto nextID{counter ? std::stoull(*counter) + 1 : 1};

        base.storage_->write(counterKey, std::to_string(nextID));

        return nextID;
    }

  public:
    MultiItemRepository(std::shared_ptr<hbt::store::StorageEngine> storage)
        : base{storage} {}

  public:
    [[nodiscard]] auto save(const T &data) -> TID {
        auto id{generateID()};

        base.storage_->write(std::to_string(id), base.serialize(data));

        return id;
    }

    [[nodiscard]] auto load(const TID &id) const -> std::optional<T> {
        auto value{base.storage_.read(std::to_string(id))};

        if (!value) {
            return std::nullopt;
        }

        return base.deserialize(*value);
    }

    auto remove(const TID &id) -> void {
        base.storage_->remove(std::to_string(id));
    }

    [[nodiscard]] auto exists(const TID &id) const -> bool {
        return base.storage_->exists(std::to_string(id));
    }
};
} // namespace hbt::repo::json
