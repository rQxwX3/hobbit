#pragma once

#include <datetime.hpp>
#include <recurrence_patterns.hpp>
#include <singular_task.hpp>
#include <task_data.hpp>
#include <task_override.hpp>

#include <optional>
#include <variant>

namespace hbt::mods {
class TaskSeries {
  public:
    using recurrencePattern_t =
        std::variant<hbt::mods::util::IntervalRecurrence,
                     hbt::mods::util::WeekdayRecurrence>;
    using deadline_t = TaskData::deadline_t;

    using start_t = TaskData::start_t;
    using stop_t = std::optional<hbt::mods::DateTime>;

    using uuid_t = std::string;

  private:
    static constexpr auto invalidDeadlineError{std::string_view{
        "Recurrent tasks can only have interval-based deadlines"}};

    static constexpr auto invalidStartFromError{std::string_view{
        "Recurrent tasks can't start after they stop repeating"}};

    static constexpr auto invalidRepeatUntilError{std::string_view{
        "Recurrent tasks can't stop repeating before they start"}};

  private:
    TaskData task_;

    recurrencePattern_t recurrencePattern_;

    stop_t stop_;

    uuid_t uuid_;

  public:
    TaskSeries(const TaskData &taskData, recurrencePattern_t recurrencePattern,
               stop_t stop = std::nullopt);

  private:
    [[nodiscard]] auto validateTaskData(const TaskData &task) const -> TaskData;

    auto validateDeadline(deadline_t deadline) const -> deadline_t;

    auto validateStart(start_t start) const -> start_t;

    auto validateStop(stop_t stop) const -> stop_t;

  private:
    [[nodiscard]] auto
    generateFirstSingularOfDate(mods::DateTime datetime) const
        -> std::optional<hbt::mods::SingularTask>;

  public:
    [[nodiscard]] auto generateSingularsForDate(mods::DateTime datetime) const
        -> std::vector<hbt::mods::SingularTask>;

  public:
    auto setStart(start_t start) -> void;

    auto setStop(stop_t stop) -> void;

    auto setDeadline(deadline_t deadline) -> void;

    auto setRecurrencePattern(recurrencePattern_t recurrencePattern);

  public:
    [[nodiscard]] auto getRecurrencePattern() const -> recurrencePattern_t;

    [[nodiscard]] auto getStop() const -> stop_t;

    [[nodiscard]] auto getUUID() const -> uuid_t;

  public:
    [[nodiscard]] auto isForDate(hbt::mods::DateTime datetime) const -> bool;
};
} // namespace hbt::mods
