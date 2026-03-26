#include <entry_list_component.hpp>
#include <tui.hpp>

namespace hbt::ui::tui {
auto TUI::createMainComponent() -> ftxui::Component {
    auto component = CatchEvent(entryList_, [&](ftxui::Event event) {
        if (event == ftxui::Event::Character('q')) {
            stop();
            return true;
        }

        return false;
    });

    return component;
}

TUI::TUI()
    : screen_{ftxui::App::FullscreenAlternateScreen()},
      entryList_{std::make_shared<EntryListComponent>()},
      entries_{std::vector<hbt::mods::Entry>()} {
    entries_.push_back(hbt::mods::Entry{"todo1", {}});
    entries_.push_back(hbt::mods::Entry{"todo2", {}});
    entries_.push_back(hbt::mods::Entry{"todo3", {}});
    entryList_->setEntries(entries_);
}

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
