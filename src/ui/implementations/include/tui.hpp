#pragma once

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>

#include <date.hpp>
#include <entry.hpp>
#include <ui.hpp>

#include <vector>

namespace hbt::ui {
class TUI : public UI {
  private:
    ftxui::App screen_;

  private:
    auto createMainComponent() -> ftxui::Component;

    // private:
    //   auto static restoreTerminal() -> void;

  public:
    TUI();

    ~TUI() override;

  public:
    auto start() -> void override;

    auto stop() -> void override;

    auto refresh() -> void override;

  public:
    // auto showEntryList(const std::vector<hbt::mods::Entry> &entries,
    //                    hbt::mods::Date date) -> void override;
};
} // namespace hbt::ui
