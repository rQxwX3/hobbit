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
};

template <typename T>
class SingleItemRepository : public hbt::repo::SingleItemRepository<T> {
  private:
    hbt::repo::json::Repository<T> base;

  public:
    SingleItemRepository(std::shared_ptr<hbt::store::StorageEngine> storage);

  public:
    auto save(const T &data) -> bool;

    auto load() const -> std::optional<T>;

    auto remove() -> void;

    auto exists() -> bool;
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
    [[nodiscard]] auto generateID() -> TID;

  public:
    MultiItemRepository(std::shared_ptr<hbt::store::StorageEngine> storage);

  public:
    auto save(const T &data) -> TID;

    auto load(const TID &id) -> std::optional<T>;

    auto remove(const TID &id) -> void;

    auto exists(const TID &id) -> bool;
};
} // namespace hbt::repo::json
