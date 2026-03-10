#pragma once

#include "occurence.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace hbt {
class Entry {
  public:
    using id_t = size_t;

  private:
    static id_t sIDCounter;

  private:
    std::vector<hbt::Occurence> occurences_;

    std::string title_;
    bool isCompleted_;

    id_t id_;

  public:
    Entry(std::string_view title, std::vector<hbt::Occurence> occurences);

  public:
    auto setTitle(std::string_view title) -> void;

    auto toggleIsCompleted() -> void;

  public:
    [[nodiscard]] auto getTitle() const -> std::string_view;

    [[nodiscard]] auto isCompleted() const -> bool;

    [[nodiscard]] auto getID() const -> id_t;
};
} // namespace hbt
