#pragma once

#include "entry.hpp"

#include <algorithm>
#include <vector>

namespace hbt {
class User {
  private:
    std::vector<Entry> entries_;

  public:
    template <typename... Args> auto addEntry(Args &&...args) -> void;

    auto deleteEntry(Entry::id_t id) -> void;
};
} // namespace hbt
