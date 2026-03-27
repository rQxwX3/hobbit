#include <entry_list_component.hpp>
#include <orchestrator_component.hpp>

#include <tui.hpp>

namespace hbt::ui::tui {
auto TUI::switchToScreen(UI::Screen screen) -> void {
    orchestrator_->switchToComponent(screen);
}

auto TUI::setUpOrchestrator() -> void {
    auto orchestrator{ftxui::Make<OrchestatorComponent>()};

    orchestrator->registerComponentFactory(
        Screen::EntryList,
        [this] -> ftxui::Component { return createEntryListComponent(); });

    orchestrator->registerComponentFactory(
        Screen::CreateEntry,
        [this] -> ftxui::Component { return createEntryFormComponent(); });

    orchestrator->switchToComponent(Screen::CreateEntry);

    orchestrator_ = orchestrator;
}

auto TUI::createEntryListComponent() -> ftxui::Component {
    auto entryList{ftxui::Make<EntryListComponent>()};

    entryList->setEntries(entries_);

    return entryList;
}

auto TUI::createEntryFormComponent() -> ftxui::Component {
    if (!createEntryCallback_) {
        throw std::runtime_error("Missing create entry callback");
    }

    auto entryForm{ftxui::Make<EntryFormComponent>(
        std::move(createEntryCallback_),
        [this] -> void { switchToScreen(Screen::EntryList); })};

    return entryForm;
}

auto TUI::setCreateEntryCallback(
    const createEntryCallback_t &createEntryCallback) -> void {
    createEntryCallback_ =
        [this, createEntryCallback](const std::string &title) -> void {
        createEntryCallback(title);
        switchToScreen(Screen::EntryList);
    };
}

TUI::TUI()
    : screen_{ftxui::App::FullscreenAlternateScreen()},
      entries_{std::vector<hbt::mods::Entry>()} {}

auto TUI::start() -> void {
    setUpOrchestrator();

    auto quitHandler{
        ftxui::CatchEvent(orchestrator_, [this](ftxui::Event event) {
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

auto TUI::setEntryList(const std::vector<hbt::mods::Entry> &entries) -> void {
    entries_ = entries;

    refresh();
}

auto TUI::populateEntryList(hbt::mods::Entry entry) -> void {
    entries_.push_back(entry);
}
} // namespace hbt::ui::tui
