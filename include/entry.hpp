#pragma once

#include <string>

namespace hbt {
class Entry {
  private:
    std::string title_;

  public:
    auto setTitle(const std::string_view &title) -> void;

    auto getTitle() -> std::string_view;
};
} // namespace hbt
