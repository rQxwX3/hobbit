#include <entry_list_component.hpp>
#include <orchestrator_component.hpp>

#include <tui.hpp>

namespace hbt::ui::tui {
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

auto TUI::setupOrchestrator() -> void {
    auto orchestrator{ftxui::Make<OrchestatorComponent>()};

    orchestrator->registerComponentFactory(
        Screen::EntryList,
        [this] -> ftxui::Component { return createEntryListComponent(); });

    orchestrator->registerComponentFactory(
        Screen::CreateEntry,
        [this] -> ftxui::Component { return createEntryFormComponent(); });

    orchestrator->switchToComponent(Screen::EntryList);

    orchestrator_ = orchestrator;
}

auto TUI::setCreateEntryCallback(
    const createEntryCallback_t &createEntryCallback) -> void {
    createEntryCallback_ =
        [this, createEntryCallback](const std::string &title) -> void {
        createEntryCallback(title);
        orchestrator_->invalidateComponent(Screen::EntryList);
        switchToScreen(Screen::EntryList);
    };
}

auto TUI::start() -> void {
    setupOrchestrator();

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

TUI::TUI() : screen_{ftxui::App::FullscreenAlternateScreen()} {}

auto TUI::setEntryList(std::vector<hbt::mods::Entry> entries) -> void {
    entries_ = std::move(entries);

    refresh();
}

auto TUI::populateEntryList(hbt::mods::Entry entry) -> void {
    entries_.push_back(entry);
}

auto TUI::switchToScreen(UI::Screen screen) -> void {
    orchestrator_->switchToComponent(screen);
}

auto TUI::stop() -> void { screen_.Exit(); }

auto TUI::refresh() -> void { screen_.PostEvent(ftxui::Event::Custom); }

} // namespace hbt::ui::tui
