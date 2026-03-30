#pragma once

#include <date.hpp>
#include <entry.hpp>

#include <vector>

namespace hbt::ui {
class UI {
  public:
    using createEntryCallback_t = std::function<void(const std::string &)>;

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
    virtual auto
    setCreateEntryCallback(const createEntryCallback_t &createEntryCallback)
        -> void = 0;

    virtual auto setEntryList(std::vector<hbt::mods::Entry> entries)
        -> void = 0;

    virtual auto populateEntryList(hbt::mods::Entry entry) -> void = 0;
};
} // namespace hbt::ui
