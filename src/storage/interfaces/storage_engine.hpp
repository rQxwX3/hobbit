#pragma once

#include <optional>
#include <vector>

namespace hbt::store {
class StorageEngine {
  public:
    virtual auto write(const std::string &key, const std::string &value)
        -> void = 0;

    [[nodiscard]] virtual auto read(const std::string &key) const
        -> std::optional<std::string> = 0;

    virtual auto remove(const std::string &key) -> void = 0;

    [[nodiscard]] virtual auto exists(const std::string &key) const -> bool = 0;

  public:
    [[nodiscard]] virtual auto getCount() const -> size_t = 0;

    [[nodiscard]] virtual auto getKeyValuePairs() const
        -> std::vector<std::pair<std::string, std::string>> = 0;

    virtual auto clear() -> void = 0;

  public:
    virtual ~StorageEngine() = default;
};
} // namespace hbt::store
