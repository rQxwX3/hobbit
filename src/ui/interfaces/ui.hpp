#pragma once

#include <date.hpp>
#include <entry.hpp>

#include <vector>

namespace hbt::ui {
class UI {
  public:
    virtual ~UI() = default;

  public:
    virtual auto start() -> void = 0;

    virtual auto stop() -> void = 0;

    virtual auto refresh() -> void = 0;

  public:
    virtual auto showEntryList(const std::vector<hbt::mods::Entry> &entries,
                               hbt::mods::Date date) -> void = 0;
};
} // namespace hbt::ui
