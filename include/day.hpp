#pragma once

#include "../include/entry.hpp"
#include <chrono>
#include <vector>

namespace hbt {
class Day {
  private:
    std::chrono::weekday weekday_;
    std::vector<hbt::Entry> entries_;

  public:
    Day(std::chrono::system_clock::time_point time, std::vector<Entry> entries);

    ~Day() = default;

  public:
    Day() = delete;
    Day(const Day &) = delete;
    Day(Day &&) = delete;

    auto operator=(const Day &) -> Day & = delete;
    auto operator=(Day &&) -> Day & = delete;
};
} // namespace hbt
