#include <app.hpp>

namespace hbt::core {
App::App(std::unique_ptr<hbt::core::EntryService> entries,
         std::unique_ptr<hbt::ui::UI> ui)
    : entries_{std::move(entries)}, ui_{std::move(ui)},
      date_{hbt::mods::Date::today()} {

    ui_->setCreateEntryCallback([this](const std::string &title) -> void {
        entries_->createEntry(
            title, {hbt::mods::Occurrence{hbt::mods::Date::today(),
                                          hbt::mods::Interval::years(1)}});
    });
};

auto App::run() -> void {
    ui_->start();

    refreshUI();
}

auto App::stop() -> void { ui_->stop(); }

auto App::refreshUI() -> void {
    ui_->showEntryList(entries_->getEntriesForDate(date_));
}

} // namespace hbt::core
