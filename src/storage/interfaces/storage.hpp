#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace hbt::store {
class Storage {
  public:
    using key_t = std::string;
    using value_t = std::string;
    using pairs_t = std::unordered_map<key_t, value_t>;

  public:
    virtual auto write(const key_t &key, const value_t &value) -> void = 0;

    [[nodiscard]] virtual auto read(const key_t &key) const
        -> std::optional<value_t> = 0;

    virtual auto remove(const key_t &key) -> void = 0;

    [[nodiscard]] virtual auto exists(const key_t &key) const -> bool = 0;

  public:
    [[nodiscard]] virtual auto getKeyValuePairs() const -> pairs_t = 0;

    virtual auto clear() -> void = 0;

  public:
    virtual ~Storage() = default;
};
} // namespace hbt::store
