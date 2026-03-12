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
  protected:
    std::shared_ptr<hbt::store::StorageEngine> storage_;

  protected:
    [[nodiscard]] auto serialize(const T &data) const -> std::string {
        return data.toJSON().dump();
    }

    [[nodiscard]] auto deserialize(const std::string &raw) const
        -> std::optional<T> {
        if (raw.empty()) {
            return std::nullopt;
        }

        return T::fromJSON(nlohmann::json::parse(raw));
    }

  public:
    Repository(std::shared_ptr<hbt::store::StorageEngine> storage);

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
        : base{storage} {}

  public:
    [[nodiscard]] auto save(const T &data) -> bool {
        return base.storage_->write("1", base.serialize(data));
    }

    [[nodiscard]] auto load() const -> std::optional<T> {
        auto value{base.storage_->read("1")};

        if (!value) {
            return std::nullopt;
        }

        return base.deserialize(*value);
    }

    auto remove() -> void { base.storage_->remove("1"); }

    [[nodiscard]] auto exists() const -> bool {
        return base.storage_->exists("1");
    }
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
