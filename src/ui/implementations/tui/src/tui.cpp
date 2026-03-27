#include <entry_list_component.hpp>
#include <orchestrator_component.hpp>

#include <tui.hpp>
#include <utility>

namespace hbt::ui::tui {
auto TUI::navigateTo(UI::Screen screen) -> void {}

auto TUI::createEntryListComponent() -> ftxui::Component {
    auto entryList{ftxui::Make<EntryListComponent>()};

    entries_.push_back(hbt::mods::Entry{"todo1", {}});
    entries_.push_back(hbt::mods::Entry{"todo2", {}});
    entries_.push_back(hbt::mods::Entry{"todo3", {}});

    entryList->setEntries(entries_);

    return entryList;
}

auto TUI::createEntryFormComponent() -> ftxui::Component {
    auto entryForm{
        ftxui::Make<EntryFormComponent>(std::move(onCreateEntryCallback_))};

    return entryForm;
}

auto TUI::createMainComponent() -> ftxui::Component {
    auto orchestrator{ftxui::Make<OrchestatorComponent>()};

    orchestrator->registerComponentFactory(
        Screen::EntryList,
        [this] -> ftxui::Component { return createEntryListComponent(); });

    orchestrator->registerComponentFactory(
        Screen::CreateEntry,
        [this] -> ftxui::Component { return createEntryFormComponent(); });

    orchestrator->switchToComponent(Screen::CreateEntry);

    return orchestrator;
}

TUI::TUI(onCreateEntryCallback_t onCreateEntryCallback)
    : screen_{ftxui::App::FullscreenAlternateScreen()},
      entries_{std::vector<hbt::mods::Entry>()},
      onCreateEntryCallback_{std::move(onCreateEntryCallback)} {}

auto TUI::start() -> void {
    auto mainComponent(createMainComponent());

    auto quitHandler{
        ftxui::CatchEvent(mainComponent, [this](ftxui::Event event) {
            if (event == ftxui::Event::Character('q') ||
                event == ftxui::Event::Character('Q')) {
                stop();
                return true;
            }
            return false;
        })};

    screen_.Loop(quitHandler);
}

auto TUI::stop() -> void { screen_.Exit(); }

auto TUI::refresh() -> void { screen_.PostEvent(ftxui::Event::Custom); }

auto TUI::showEntryList(const std::vector<hbt::mods::Entry> &entries) -> void {
    entries_ = entries;

    refresh();
}
} // namespace hbt::ui::tui
