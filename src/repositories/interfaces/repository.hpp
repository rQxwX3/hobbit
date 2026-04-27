#pragma once

#include <optional>
#include <vector>

#include <uuid.hpp>

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

    virtual auto update(const T &data) -> void = 0;

    [[nodiscard]] virtual auto load() const -> std::optional<T> = 0;

    virtual auto remove() -> void = 0;

    [[nodiscard]] virtual auto exists() const -> bool = 0;
};

template <typename T> class MultiItemRepository : public Repository<T> {
  public:
    using uuid_t = core::uuid::uuid_t;

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
    virtual auto save(const T &data) -> uuid_t = 0;

    virtual auto save(T &&data) -> uuid_t = 0;

    virtual auto update(const uuid_t &id, const T &data) -> void = 0;

    virtual auto remove(const uuid_t &id) -> void = 0;

    [[nodiscard]] virtual auto exists(const uuid_t &id) const -> bool = 0;

  public:
    [[nodiscard]] virtual auto getByID(const uuid_t &id) const
        -> std::optional<T> = 0;

    [[nodiscard]] auto virtual getAll() const -> std::vector<T> = 0;
};
} // namespace hbt::repo
