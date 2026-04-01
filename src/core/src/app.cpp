#include <app.hpp>

namespace hbt::core {
App::App(std::unique_ptr<hbt::core::EntryService> entries,
         std::unique_ptr<hbt::ui::UI> ui)
    : entries_{std::move(entries)}, ui_{std::move(ui)},
      date_{hbt::mods::Date::today()} {

    ui_->setCreateEntryCallback(
        [this](std::string title, hbt::mods::Interval interval) -> void {
            auto entry{hbt::mods::Entry{
                std::move(title),
                {hbt::mods::Occurrence{hbt::mods::Date::today(), interval}}}};

            entries_->createEntry(entry);
            ui_->populateEntryList(entry);
        });

    ui_->setEntryList(entries_->getEntriesForDate(date_));
};

auto App::run() -> void { ui_->start(); }

auto App::stop() -> void { ui_->stop(); }
} // namespace hbt::core
