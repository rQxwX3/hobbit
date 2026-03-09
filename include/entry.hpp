#pragma once

#include "occurence.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace hbt {
class Entry {
  private:
    std::vector<hbt::Occurence> occurences_;

    std::string title_;
    bool isCompleted_;

  public:
    Entry(std::string_view title, std::vector<hbt::Occurence> occurences);

  public:
    auto setTitle(std::string_view title) -> void;

    auto toggleIsCompleted() -> void;

  public:
    [[nodiscard]] auto getTitle() const -> std::string_view;

    [[nodiscard]] auto isCompleted() const -> bool;
};
} // namespace hbt
