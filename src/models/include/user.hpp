#pragma once

#include "entry.hpp"

#include <vector>

namespace hbt {
class User {
  private:
    std::vector<Entry> entries_;

  public:
    User() = default;

    User(const User &) = delete;
    User(User &&) = delete;

    auto operator=(const User &) -> User & = delete;
    auto operator=(User &&) -> User & = delete;

  public:
    template <typename... Args> auto addEntry(Args &&...args) -> void {
        entries_.emplace_back(std::forward<Args>(args)...);
    }

    auto deleteEntry(Entry::id_t id) -> void;

  public:
    [[nodiscard]] auto getEntries() & -> const std::vector<Entry> &;
};
} // namespace hbt
