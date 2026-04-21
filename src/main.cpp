#include <app.hpp>
#include <json_repository.hpp>
#include <json_storage_engine.hpp>
#include <task_manager.hpp>
#include <tui.hpp>

#include <memory>

auto main() -> int {
    auto storage{
        std::make_shared<hbt::store::json::StorageEngine>("tasks.json")};

    auto tasksRepo{std::make_unique<
        hbt::repo::json::MultiItemRepository<hbt::mods::TaskData>>(storage)};

    auto app{hbt::core::App{
        std::make_unique<hbt::core::TaskManager>(std::move(tasksRepo)),
        std::make_unique<hbt::ui::tui::TUI>()}};

    app.run();
}
