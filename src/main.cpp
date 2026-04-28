#include <app.hpp>
#include <json_repository.hpp>
#include <json_storage.hpp>
#include <task_manager.hpp>
#include <tui.hpp>

#include <memory>

auto main() -> int {
    // auto storage{std::make_shared<hbt::store::json::Storage>("tasks.json")};
    //
    // auto singulars{std::make_unique<
    //     hbt::repo::json::MultiItemRepository<hbt::mods::SingularTask>>(
    //     storage)};
    //
    // auto series{std::make_unique<
    //     hbt::repo::json::MultiItemRepository<hbt::mods::TaskSeries>>(storage)};
    //
    // auto app{hbt::core::App{std::make_unique<hbt::core::TaskManager>(
    //                             std::move(series), std::move(singulars)),
    //                         std::make_unique<hbt::ui::tui::TUI>()}};
    //
    // app.run();
}
