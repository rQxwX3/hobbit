#pragma once

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>

#include <date.hpp>
#include <entry.hpp>
#include <ui.hpp>

#include <entry_list_component.hpp>

#include <memory>
#include <vector>

namespace hbt::ui::tui {
class TUI : public UI {
  private:
    ftxui::App screen_;

    std::shared_ptr<EntryListComponent> entryList_;

    std::vector<hbt::mods::Entry> entries_;

  private:
    auto createMainComponent() -> ftxui::Component;

    // private:
    //   auto static restoreTerminal() -> void;

  public:
    TUI();

  public:
    auto start() -> void override;

    auto stop() -> void override;

    auto refresh() -> void override;

  public:
    auto showEntryList(const std::vector<hbt::mods::Entry> &entries)
        -> void override;
};
} // namespace hbt::ui::tui
