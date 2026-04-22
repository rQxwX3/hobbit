#include <algorithm>

#include <datetime.hpp>
#include <task_manager.hpp>

namespace hbt::core {
[[nodiscard]] auto
TaskManager::instantiateSeriesForDate(mods::DateTime datetime) const
    -> singulars_t {
    auto result{singulars_t{}};

    auto series{seriesRepo_->getAll()};

    for (const auto &s : series) {
        auto singulars{s.generateSingularsForDate(datetime)};

        result.insert(result.end(), std::make_move_iterator(singulars.begin()),
                      std::make_move_iterator(singulars.end()));
    }

    return result;
}

TaskManager::TaskManager(series_repo_t seriesRepo,
                         singulars_repo_t singularsRepo)
    : seriesRepo_{std::move(seriesRepo)},
      singularsRepo_{std::move(singularsRepo)} {}

[[nodiscard]] auto TaskManager::getTasksForDate(mods::DateTime datetime) const
    -> singulars_t {
    auto result{singulars_t{}};
    auto singulars{singularsRepo_->getAll()};

    for (const auto &singular : singulars) {
        if (singular.isForDate(datetime)) {
            result.push_back(singular);
        }
    }

    auto instantiatedSeries{instantiateSeriesForDate(datetime)};
    result.insert(result.end(),
                  std::make_move_iterator(instantiatedSeries.begin()),
                  std::make_move_iterator(instantiatedSeries.end()));

    return result;
}

// auto TaskManager::createTask(const hbt::mods::Task &task) -> id_t {
//     return repository_->save(task);
// }
//
// auto TaskManager::createTask(std::string title,
//                              std::vector<hbt::mods::Occurrence> occurrences)
//     -> id_t {
//     return repository_->save(
//         hbt::mods::Task{std::move(title), std::move(occurrences)});
// }
//
// auto TaskManager::deleteTask(id_t id) -> void { repository_->remove(id); }
//
// auto TaskManager::changeTaskTitle(id_t id, std::string title) -> void {
//     auto taskOpt{repository_->load(id)};
//
//     if (taskOpt.has_value()) {
//         auto task{std::move(taskOpt.value())};
//
//         task.setTitle(std::move(title));
//
//         repository_->update(id, task);
//     }
// }
//
// auto TaskManager::changeTaskOccurrences(
//     id_t id, std::vector<hbt::mods::Occurrence> occurrences) -> void {
//     auto taskOpt{repository_->load(id)};
//
//     if (taskOpt.has_value()) {
//         auto task{std::move(taskOpt.value())};
//
//         task.setOccurrences(std::move(occurrences));
//
//         repository_->update(id, task);
//     }
// }
//
// auto TaskManager::completeTask(id_t id) -> void {
//     auto taskOpt{repository_->load(id)};
//
//     if (taskOpt.has_value()) {
//         auto task{std::move(taskOpt.value())};
//
//         task.setIsCompleted(true);
//
//         repository_->update(id, task);
//     }
// }
//
// auto TaskManager::uncompleteTask(id_t id) -> void {
//     auto taskOpt{repository_->load(id)};
//
//     if (taskOpt.has_value()) {
//         auto task{std::move(taskOpt.value())};
//
//         task.setIsCompleted(false);
//
//         repository_->update(id, task);
//     }
// }
//
// [[nodiscard]] auto
// TaskManager::getTasksForDate(const mods::DateTime &date) const
//     -> std::vector<hbt::mods::Task> {
//     auto tasksForDate{std::vector<hbt::mods::Task>()};
//     const auto &allTasks{repository_->getAll()};
//
//     std::ranges::copy_if(allTasks, std::back_inserter(tasksForDate),
//                          [date](const hbt::mods::Task &task) -> bool {
//                              return task.isForDate(date);
//                          });
//
//     return tasksForDate;
// }
//
// [[nodiscard]] auto TaskManager::getCount() const -> size_t {
//     return repository_->getCount();
// }
} // namespace hbt::core
