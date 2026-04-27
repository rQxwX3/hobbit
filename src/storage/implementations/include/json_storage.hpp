#pragma once

#include <storage.hpp>

#include <string>

namespace hbt::store::json {
class Storage : public hbt::store::Storage {
  public:
    using key_t = Storage::key_t;
    using value_t = Storage::value_t;
    using pairs_t = Storage::pairs_t;

  private:
    std::string filename_;

    pairs_t pairs_;

  private:
    auto load() -> void;

    auto save() -> void;

  public:
    explicit Storage(std::string filename);

  public:
    auto write(const key_t &key, const value_t &value) -> void override;

    [[nodiscard]] auto read(const key_t &key) const
        -> std::optional<value_t> override;

    auto remove(const key_t &key) -> void override;

    [[nodiscard]] auto exists(const key_t &key) const -> bool override;

  public:
    [[nodiscard]] auto getKeyValuePairs() const -> pairs_t override;

    auto clear() -> void override;
};
} // namespace hbt::store::json
