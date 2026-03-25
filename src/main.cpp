#include <app.hpp>
#include <entry_service.hpp>
#include <json_repository.hpp>
#include <json_storage_engine.hpp>
#include <tui.hpp>

#include <memory>

auto main() -> int {
    auto storage{
        std::make_shared<hbt::store::json::StorageEngine>("entries.json")};

    auto entriesRepo{std::make_unique<
        hbt::repo::json::MultiItemRepository<hbt::mods::Entry>>(storage)};

    auto entries{
        std::make_unique<hbt::core::EntryService>(std::move(entriesRepo))};

    auto tui{std::make_unique<hbt::ui::tui::TUI>()};

    auto app{hbt::core::App{std::move(entries), std::move(tui)}};

    app.run();
    app.refreshUI();
}
