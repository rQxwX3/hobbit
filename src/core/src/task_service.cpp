#include <algorithm>

#include <datetime.hpp>
#include <task_service.hpp>

namespace hbt::core {
TaskService::TaskService(
    std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::Task>> repository)
    : repository_{std::move(repository)} {}

auto TaskService::createTask(const hbt::mods::Task &task) -> id_t {
    return repository_->save(task);
}

auto TaskService::createTask(std::string title,
                             std::vector<hbt::mods::Occurrence> occurrences)
    -> id_t {
    return repository_->save(
        hbt::mods::Task{std::move(title), std::move(occurrences)});
}

auto TaskService::deleteTask(id_t id) -> void { repository_->remove(id); }

auto TaskService::changeTaskTitle(id_t id, std::string title) -> void {
    auto taskOpt{repository_->load(id)};

    if (taskOpt.has_value()) {
        auto task{std::move(taskOpt.value())};

        task.setTitle(std::move(title));

        repository_->update(id, task);
    }
}

auto TaskService::changeTaskOccurrences(
    id_t id, std::vector<hbt::mods::Occurrence> occurrences) -> void {
    auto taskOpt{repository_->load(id)};

    if (taskOpt.has_value()) {
        auto task{std::move(taskOpt.value())};

        task.setOccurrences(std::move(occurrences));

        repository_->update(id, task);
    }
}

auto TaskService::completeTask(id_t id) -> void {
    auto taskOpt{repository_->load(id)};

    if (taskOpt.has_value()) {
        auto task{std::move(taskOpt.value())};

        task.setIsCompleted(true);

        repository_->update(id, task);
    }
}

auto TaskService::uncompleteTask(id_t id) -> void {
    auto taskOpt{repository_->load(id)};

    if (taskOpt.has_value()) {
        auto task{std::move(taskOpt.value())};

        task.setIsCompleted(false);

        repository_->update(id, task);
    }
}

[[nodiscard]] auto
TaskService::getTasksForDate(const mods::DateTime &date) const
    -> std::vector<hbt::mods::Task> {
    auto tasksForDate{std::vector<hbt::mods::Task>()};
    const auto &allTasks{repository_->getAll()};

    std::ranges::copy_if(allTasks, std::back_inserter(tasksForDate),
                         [date](const hbt::mods::Task &task) -> bool {
                             return task.isForDate(date);
                         });

    return tasksForDate;
}

[[nodiscard]] auto TaskService::getCount() const -> size_t {
    return repository_->getCount();
}
} // namespace hbt::core
