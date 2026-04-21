#include <recurrent_task.hpp>
#include <uuid.hpp>

namespace hbt::mods {
auto RecurrentTask::validateDeadline(deadline_t deadline) const -> deadline_t {
    if (!std::holds_alternative<hbt::mods::Interval>(deadline)) {
        throw std::invalid_argument(std::string{invalidDeadlineError});
    }

    return deadline;
}

auto RecurrentTask::validateStartFrom(hbt::mods::DateTime startFrom) const
    -> startFrom_t {
    if (repeatUntil_.has_value() && startFrom > repeatUntil_.value()) {
        throw std::invalid_argument(std::string{invalidStartFromError});
    }

    return startFrom;
}

auto RecurrentTask::validateRepeatUntil(repeatUntil_t repeatUntil) const
    -> repeatUntil_t {
    if (repeatUntil.has_value() && repeatUntil.value() < task_.startFrom) {
        throw std::invalid_argument(std::string{invalidRepeatUntilError});
    }

    return repeatUntil;
}

[[nodiscard]] auto RecurrentTask::validateTaskData(const TaskData &task) const
    -> TaskData {
    validateDeadline(task.deadline);
    validateStartFrom(task.startFrom);

    return task;
}

RecurrentTask::RecurrentTask(const TaskData &task,
                             recurrencePattern_t recurrencePattern,
                             repeatUntil_t repeatUntil)
    : task_(validateTaskData(task)),
      recurrencePattern_{std::move(recurrencePattern)},
      repeatUntil_{validateRepeatUntil(repeatUntil)},
      uuid_{core::uuid::generateUUID()} {}

auto RecurrentTask::setStartFrom(startFrom_t startFrom) -> void {
    task_.startFrom = validateStartFrom(startFrom);
}

auto RecurrentTask::setDeadline(deadline_t deadline) -> void {
    task_.deadline = validateDeadline(std::move(deadline));
}

auto RecurrentTask::setRecurrencePattern(
    recurrencePattern_t recurrencePattern) {
    recurrencePattern_ = std::move(recurrencePattern);
}

auto RecurrentTask::setRepeatUntil(repeatUntil_t repeatUntil) {
    repeatUntil_ = validateRepeatUntil(repeatUntil);
}

[[nodiscard]] auto RecurrentTask::getRecurrencePattern() const
    -> recurrencePattern_t {
    return recurrencePattern_;
}

[[nodiscard]] auto RecurrentTask::getRepeatUntil() const -> repeatUntil_t {
    return repeatUntil_;
}

[[nodiscard]] auto RecurrentTask::isForDate(DateTime datetime) const -> bool {
    if (datetime < task_.startFrom ||
        (repeatUntil_.has_value() && datetime > repeatUntil_.value())) {
        return false;
    }

    if (std::holds_alternative<util::IntervalRecurrence>(recurrencePattern_)) {
        return std::get<util::IntervalRecurrence>(recurrencePattern_)
            .happensOnDate(task_.startFrom, datetime);
    }

    if (std::holds_alternative<util::WeekdayRecurrence>(recurrencePattern_)) {
        return std::get<util::WeekdayRecurrence>(recurrencePattern_)
            .happensOnDate(datetime);
    }

    return false;
}
} // namespace hbt::mods
