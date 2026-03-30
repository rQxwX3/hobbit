#pragma once

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>

#include <date.hpp>
#include <entry.hpp>
#include <ui.hpp>

#include <entry_form_component.hpp>
#include <entry_list_component.hpp>
#include <orchestrator_component.hpp>

#include <vector>

namespace hbt::ui::tui {
class TUI : public UI {
  private:
    std::vector<hbt::mods::Entry> entries_;

    ftxui::App screen_;

    std::shared_ptr<OrchestatorComponent> orchestrator_;

    createEntryCallback_t createEntryCallback_;

  private:
    auto switchToScreen(UI::Screen screen) -> void;

  private:
    auto createEntryListComponent() -> ftxui::Component;

    auto createEntryFormComponent() -> ftxui::Component;

    auto setupOrchestrator() -> void;

    // private:
    //   auto static restoreTerminal() -> void;

  public:
    TUI();

  public:
    auto
    setCreateEntryCallback(const createEntryCallback_t &createEntryCallback)
        -> void override;

    auto setEntryList(std::vector<hbt::mods::Entry> entries) -> void override;

    auto populateEntryList(hbt::mods::Entry entry) -> void override;

  public:
    auto start() -> void override;

    auto stop() -> void override;

    auto refresh() -> void override;

  public:
};
} // namespace hbt::ui::tui
