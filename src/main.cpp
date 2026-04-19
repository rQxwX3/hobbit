#include <app.hpp>
#include <json_repository.hpp>
#include <json_storage_engine.hpp>
#include <task_service.hpp>
#include <tui.hpp>

#include <memory>

auto main() -> int {
    auto storage{
        std::make_shared<hbt::store::json::StorageEngine>("tasks.json")};

    auto tasksRepo{
        std::make_unique<hbt::repo::json::MultiItemRepository<hbt::mods::Task>>(
            storage)};

    auto app{hbt::core::App{
        std::make_unique<hbt::core::TaskService>(std::move(tasksRepo)),
        std::make_unique<hbt::ui::tui::TUI>()}};

    app.run();
}
