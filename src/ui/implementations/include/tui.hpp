#pragma once

#include <ftxui/component/component.hpp>

#include <date.hpp>
#include <entry.hpp>
#include <ui.hpp>

#include <vector>

namespace hbt::ui {
class TUI : public UI {
  public:
    auto start() -> void override;

    auto stop() -> void override;

    auto refresh() -> void override;

  public:
    auto showEntryList(const std::vector<hbt::mods::Entry> &entries,
                       hbt::mods::Date date) -> void override;
};
} // namespace hbt::ui
