#pragma once

#include <storage_engine.hpp>

#include <string>
#include <unordered_map>

namespace hbt::store::json {
class StorageEngine : public hbt::store::StorageEngine {
  private:
    using data_t = std::unordered_map<std::string, std::string>;

  private:
    std::string filename_;

    data_t data_;

  private:
    auto load() -> void;

    auto save() -> void;

  public:
    explicit StorageEngine(std::string filename);

  public:
    auto write(const std::string &key, const std::string &value)
        -> void override;

    [[nodiscard]] auto read(const std::string &key) const
        -> std::optional<std::string> override;

    auto remove(const std::string &key) -> void override;

    [[nodiscard]] auto exists(const std::string &key) const -> bool override;

  public:
    [[nodiscard]] auto getCount() const -> size_t override;

    [[nodiscard]] auto getKeyValuePairs() const -> data_t override;

    auto clear() -> void override;
};

} // namespace hbt::store::json
