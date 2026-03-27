#pragma once

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>

#include <date.hpp>
#include <entry.hpp>
#include <ui.hpp>

#include <entry_form_component.hpp>
#include <entry_list_component.hpp>

#include <functional>
#include <vector>

namespace hbt::ui::tui {
class TUI : public UI {
  public:
    using onCreateEntryCallback_t = std::function<void(const std::string &)>;

  private:
    std::vector<hbt::mods::Entry> entries_;

    ftxui::App screen_;

    onCreateEntryCallback_t onCreateEntryCallback_;

  private:
    auto navigateTo(UI::Screen screen) -> void;

  private:
    auto createMainComponent() -> ftxui::Component;

    auto createEntryListComponent() -> ftxui::Component;

    auto createEntryFormComponent() -> ftxui::Component;

    // private:
    //   auto static restoreTerminal() -> void;

  public:
    TUI(onCreateEntryCallback_t onCreateEntryCallback);

  public:
    auto start() -> void override;

    auto stop() -> void override;

    auto refresh() -> void override;

  public:
    auto showEntryList(const std::vector<hbt::mods::Entry> &entries)
        -> void override;
};
} // namespace hbt::ui::tui
