#pragma once

#include <optional>
#include <vector>

namespace hbt::repo {
template <typename T> class Repository {
  public:
    virtual auto getAll() -> std::vector<T> = 0;

    virtual auto getCount() -> size_t = 0;

    virtual auto clear() -> void = 0;

  public:
    virtual ~Repository() = default;

  public:
    Repository(const Repository &) = delete;
    Repository(Repository &&) = delete;

    auto operator=(const Repository &) -> Repository & = delete;
    auto operator=(Repository &&) -> Repository & = delete;
};

template <typename T> class SingleItemRepository : public Repository<T> {
  public:
    virtual auto save(const T &data) -> bool = 0;

    virtual auto load() const -> std::optional<T> = 0;

    virtual auto remove() -> void = 0;

    virtual auto exists() -> bool = 0;

  public:
    virtual ~SingleItemRepository() = default;

  public:
    SingleItemRepository(const SingleItemRepository &) = delete;
    SingleItemRepository(SingleItemRepository &&) = delete;

    auto operator=(const SingleItemRepository &)
        -> SingleItemRepository & = delete;
    auto operator=(SingleItemRepository &&) -> SingleItemRepository & = delete;
};

template <typename T, typename TID>
class MultiItemRepository : public Repository<T> {
  public:
    virtual auto save(const T &data) -> TID = 0;

    virtual auto load(const TID &id) -> std::optional<T> = 0;

    virtual auto remove(const TID &id) -> void = 0;

    virtual auto exists(const TID &id) -> bool = 0;

  public:
    virtual ~MultiItemRepository() = default;

  public:
    MultiItemRepository(const MultiItemRepository &) = delete;
    MultiItemRepository(MultiItemRepository &&) = delete;

    auto operator=(const MultiItemRepository &)
        -> MultiItemRepository & = delete;
    auto operator=(MultiItemRepository &&) -> MultiItemRepository & = delete;
};
} // namespace hbt::repo
