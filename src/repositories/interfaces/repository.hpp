#pragma once

#include <optional>
#include <vector>

namespace hbt::repo {
template <typename T> class Repository {
  public:
    Repository() = default;

    virtual ~Repository() = default;

  public:
    Repository(const Repository &) = delete;
    Repository(Repository &&) = delete;

    auto operator=(const Repository &) -> Repository & = delete;
    auto operator=(Repository &&) -> Repository & = delete;

  public:
    [[nodiscard]] virtual auto getAll() const -> std::vector<T> = 0;

    [[nodiscard]] virtual auto getCount() const -> size_t = 0;

    virtual auto clear() -> void = 0;
};

template <typename T> class SingleItemRepository : public Repository<T> {
  public:
    SingleItemRepository() = default;

    virtual ~SingleItemRepository() = default;

  public:
    SingleItemRepository(const SingleItemRepository &) = delete;
    SingleItemRepository(SingleItemRepository &&) = delete;

    auto operator=(const SingleItemRepository &)
        -> SingleItemRepository & = delete;
    auto operator=(SingleItemRepository &&) -> SingleItemRepository & = delete;

  public:
    virtual auto save(const T &data) -> void = 0;

    virtual auto save(T &&data) -> void = 0;

    [[nodiscard]] virtual auto load() const -> std::optional<T> = 0;

    virtual auto remove() -> void = 0;

    [[nodiscard]] virtual auto exists() const -> bool = 0;
};

template <typename T, typename TID = size_t>
class MultiItemRepository : public Repository<T> {
  public:
    MultiItemRepository() = default;

    virtual ~MultiItemRepository() = default;

  public:
    MultiItemRepository(const MultiItemRepository &) = delete;
    MultiItemRepository(MultiItemRepository &&) = delete;

    auto operator=(const MultiItemRepository &)
        -> MultiItemRepository & = delete;
    auto operator=(MultiItemRepository &&) -> MultiItemRepository & = delete;

  public:
    virtual auto save(const T &data) -> TID = 0;

    virtual auto save(T &&data) -> TID = 0;

    [[nodiscard]] virtual auto load(const TID &id) const
        -> std::optional<T> = 0;

    virtual auto remove(const TID &id) -> void = 0;

    [[nodiscard]] virtual auto exists(const TID &id) const -> bool = 0;
};
} // namespace hbt::repo
