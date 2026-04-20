#pragma once

#include <datetime.hpp>
#include <repository.hpp>
#include <task.hpp>

#include <memory>

namespace hbt::core {
class TaskService {
  private:
    using id_t = size_t;

  private:
    std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::TaskData>>
        repository_;

  public:
    explicit TaskService(
        std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::TaskData>>
            repository);

  public:
    auto createTask(std::string title,
                    std::vector<hbt::mods::Occurrence> occurrences) -> id_t;

    auto createTask(const hbt::mods::TaskData &task) -> id_t;

    auto deleteTask(id_t id) -> void;

  public:
    auto changeTaskTitle(id_t id, std::string title) -> void;

    auto changeTaskOccurrences(id_t id,
                               std::vector<hbt::mods::Occurrence> occurrences)
        -> void;

    auto completeTask(id_t id) -> void;

    auto uncompleteTask(id_t id) -> void;

  public:
    [[nodiscard]] auto getTasksForDate(const mods::DateTime &date) const
        -> std::vector<hbt::mods::TaskData>;

    [[nodiscard]] auto getCount() const -> size_t;
};
} // namespace hbt::core
