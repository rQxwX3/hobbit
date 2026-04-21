#pragma once

#include <datetime.hpp>
#include <interval.hpp>
#include <repository.hpp>
#include <singular_task.hpp>
#include <task_override.hpp>
#include <task_series.hpp>

#include <memory>
#include <unordered_map>

namespace hbt::core {
class TaskManager {
  private:
    using uuid_t = std::string;

    using series_t = std::vector<hbt::mods::TaskSeries>;
    using singulars_t = std::vector<hbt::mods::SingularTask>;
    using overrides_t = std::vector<mods::TaskOverride>;

    using overrides_cache_t = std::map<mods::TaskSeries::uuid_t, overrides_t>;

    using singulars_repo_t = std::unique_ptr<
        hbt::repo::MultiItemRepository<hbt::mods::SingularTask>>;

    using series_repo_t =
        std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::TaskSeries>>;

    using overrides_repo_t =
        std::unique_ptr<hbt::repo::MultiItemRepository<overrides_t>>;

  private:
    static const inline auto lookaheadInterval{hbt::mods::Interval::days(90)};
    static const inline auto retentionInterval{hbt::mods::Interval::days(7)};

  private:
    series_repo_t seriesRepo_;
    singulars_repo_t singularsRepo_;
    overrides_repo_t overridesRepo_;

    series_t series_;
    singulars_t singulars_;
    overrides_cache_t overrides_;

  public:
    explicit TaskManager(series_repo_t seriesRepo,
                         singulars_repo_t singularsRepo,
                         overrides_repo_t overridesRepo);

  private:
    [[nodiscard]] auto singularsFromSeriesForDate(mods::DateTime datetime) const
        -> singulars_t;

  public:
    [[nodiscard]] auto getTasksForDate(mods::DateTime datetime) const
        -> singulars_t;

  public:
    // auto createTask(std::string title,
    //                 std::vector<hbt::mods::Occurrence> occurrences) -> id_t;

    auto createTask(const hbt::mods::TaskData &task) -> id_t;

    auto deleteTask(id_t id) -> void;

  public:
    auto changeTaskTitle(id_t id, std::string title) -> void;

    // auto changeTaskOccurrences(id_t id,
    //                            std::vector<hbt::mods::Occurrence>
    //                            occurrences)
    //     -> void;

    auto completeTask(id_t id) -> void;

    auto uncompleteTask(id_t id) -> void;

  public:
    // [[nodiscard]] auto

    [[nodiscard]] auto getCount() const -> size_t;
};
} // namespace hbt::core
