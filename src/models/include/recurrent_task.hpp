#pragma once

#include <datetime.hpp>
#include <recurrence_patterns.hpp>
#include <task_data.hpp>

#include <optional>
#include <variant>

namespace hbt::mods {
class RecurrentTask {
  public:
    virtual ~RecurrentTask() = default;
    using repeatUntil_t = std::optional<hbt::mods::DateTime>;
    using recurrencePattern_t =
        std::variant<hbt::mods::util::IntervalRecurrence,
                     hbt::mods::util::WeekdayRecurrence>;
    using deadline_t = TaskData::deadline_t;
    using startFrom_t = TaskData::startFrom_t;

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

    repeatUntil_t repeatUntil_;

  public:
    RecurrentTask(const TaskData &taskData,
                  recurrencePattern_t recurrencePattern,
                  repeatUntil_t repeatUntil = std::nullopt);

  private:
    [[nodiscard]] auto validateTaskData(const TaskData &task) const -> TaskData;

    auto validateDeadline(deadline_t deadline) const -> deadline_t;

    auto validateStartFrom(hbt::mods::DateTime startFrom) const -> startFrom_t;

    auto validateRepeatUntil(repeatUntil_t repeatUntil) const -> repeatUntil_t;

  public:
    auto setStartFrom(startFrom_t startFrom) -> void;

    auto setDeadline(deadline_t deadline) -> void;

    auto setRecurrencePattern(recurrencePattern_t recurrencePattern);

    auto setRepeatUntil(repeatUntil_t repeatUntil);

  public:
    [[nodiscard]] auto getRecurrencePattern() const -> recurrencePattern_t;

    [[nodiscard]] auto getRepeatUntil() const -> repeatUntil_t;

  public:
    [[nodiscard]] auto isForDate(hbt::mods::DateTime datetime) const -> bool;
};
} // namespace hbt::mods
