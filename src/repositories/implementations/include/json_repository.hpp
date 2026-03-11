#pragma once

#include "../../../storage/storage_engine.hpp"
#include "../../interfaces/repository.hpp"

#include <concepts>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>

namespace hbt::repo::json {
template <typename T> class Repository {
  protected:
    std::shared_ptr<hbt::store::StorageEngine> storage_;

  public:
    Repository(std::shared_ptr<hbt::store::StorageEngine> storage);

  public:
    [[nodiscard]] auto getAll() const -> std::vector<T> {
        return storage_->getAll();
    }

    [[nodiscard]] auto getCount() const -> size_t {
        return storage_->getCount();
    }

    auto clear() -> void { storage_->clear(); }
};

template <typename T>
class SingleItemRepository : public hbt::repo::SingleItemRepository<T> {
  private:
    hbt::repo::json::Repository<T> base;

  public:
    SingleItemRepository(std::shared_ptr<hbt::store::StorageEngine> storage)
        : base{storage} {}

  public:
    [[nodiscard]] auto save(const T &data) -> bool {
        return base.storage_->write(std::to_string(1));
    }

    [[nodiscard]] auto load() const -> std::optional<T> {
        return base.storage_->read(std::to_string(1));
    }

    auto remove() -> void { base.storage_.remove(std::to_string(1)); }

    [[nodiscard]] auto exists() const -> bool {
        return base.storage_->exists(std::to_string(1));
    }
};

template <typename TID>
concept IDCompatible = std::same_as<TID, std::size_t>;

template <typename T, IDCompatible TID = std::size_t>
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
        base.storage_->write(generateID(), "TODO");
    }

    [[nodiscard]] auto load(const TID &id) const -> std::optional<T>;

    auto remove(const TID &id) -> void;

    [[nodiscard]] auto exists(const TID &id) const -> bool;
};
} // namespace hbt::repo::json
