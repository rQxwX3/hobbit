#include "../include/entry.hpp"

namespace hbt {
auto Entry::setTitle(const std::string_view &title) -> void { title_ = title; }

auto Entry::getTitle() -> std::string_view { return title_; }
} // namespace hbt
