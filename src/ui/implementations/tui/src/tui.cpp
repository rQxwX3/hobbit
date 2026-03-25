#include <tui.hpp>

namespace hbt::ui::tui {
auto TUI::createMainComponent() -> ftxui::Component {}

TUI::TUI()
    : screen_{ftxui::App::FullscreenAlternateScreen()},
      entries_{std::vector<hbt::mods::Entry>()} {}

auto TUI::start() -> void {
    auto mainComponent(createMainComponent());

    screen_.Loop(mainComponent);
}

auto TUI::stop() -> void { screen_.Exit(); }

auto TUI::refresh() -> void { screen_.PostEvent(ftxui::Event::Custom); }

auto TUI::showEntryList(const std::vector<hbt::mods::Entry> &entries) -> void {
    entries_ = entries;

    refresh();
}
} // namespace hbt::ui::tui
