#pragma once

#include <datetime.hpp>
#include <interval.hpp>
#include <repository.hpp>
#include <singular_task.hpp>
#include <task_series.hpp>

#include <memory>
#include <unordered_map>

namespace hbt::core {
class TaskManager {
  private:
    using uuid_t = std::string;

    using singulars_repo_t = std::unique_ptr<
        hbt::repo::MultiItemRepository<hbt::mods::SingularTask>>;
    using series_repo_t =
        std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::TaskSeries>>;

  private:
    using singulars_t = std::vector<mods::SingularTask>;

    using calendar_t = std::unordered_map<hbt::mods::Date, singulars_t>;

  private:
    static const inline auto lookaheadInterval{hbt::mods::Interval::days(90)};
    static const inline auto retentionInterval{hbt::mods::Interval::days(7)};

  private:
    series_repo_t seriesRepo_;
    singulars_repo_t singularsRepo_;

  public:
    explicit TaskManager(series_repo_t seriesRepo,
                         singulars_repo_t singularsRepo);

  private:
    [[nodiscard]] auto instantiateSeriesForDate(mods::Date date) const
        -> singulars_t;

  public:
    [[nodiscard]] auto getTasksForDate(mods::Date date) const -> singulars_t;

    // Including from, excluding to
    [[nodiscard]] auto getTasksForDateRange(mods::Date from,
                                            mods::Date to) const -> calendar_t;

  public:
    // auto createTask(std::string title,
    //                 std::vector<hbt::mods::Occurrence> occurrences) ->
    //                 id_t;

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
