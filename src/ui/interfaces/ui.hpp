#pragma once

#include <date.hpp>
#include <entry.hpp>

#include <vector>

namespace hbt::ui {
class UI {
  public:
    enum class Screen : uint8_t {
        EntryList,
        CreateEntry,
        Statistics,
        Settings,
        NONE,
    };

  public:
    virtual ~UI() = default;

  public:
    virtual auto start() -> void = 0;

    virtual auto stop() -> void = 0;

    virtual auto refresh() -> void = 0;

  public:
    virtual auto showEntryList(const std::vector<hbt::mods::Entry> &entries)
        -> void = 0;
};
} // namespace hbt::ui
