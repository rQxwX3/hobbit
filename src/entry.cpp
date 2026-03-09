#include "../include/entry.hpp"

namespace hbt {
Entry::Entry(std::string_view title, std::vector<hbt::Occurence> occurences)
    : title_{title}, occurences_{std::move(occurences)}, isCompleted_{false} {}

auto Entry::setTitle(std::string_view title) -> void { title_ = title; }

auto Entry::toggleIsCompleted() -> void { isCompleted_ = !isCompleted_; }

[[nodiscard]] auto Entry::getTitle() const -> std::string_view {
    return title_;
}

[[nodiscard]] auto Entry::isCompleted() const -> bool { return isCompleted_; }
} // namespace hbt
