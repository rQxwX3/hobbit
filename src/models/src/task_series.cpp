#include <task_series.hpp>
#include <uuid.hpp>

namespace hbt::mods {
auto TaskSeries::validateDeadline(deadline_t deadline) const -> deadline_t {
    if (!std::holds_alternative<hbt::mods::Interval>(deadline)) {
        throw std::invalid_argument(std::string{invalidDeadlineError});
    }

    return deadline;
}

auto TaskSeries::validateStart(start_t start) const -> start_t {
    if (stop_.has_value() && start > stop_.value()) {
        throw std::invalid_argument(std::string{invalidStartFromError});
    }

    return start;
}

auto TaskSeries::validateStop(stop_t stop) const -> stop_t {
    if (stop.has_value() && stop.value() < task_.start) {
        throw std::invalid_argument(std::string{invalidRepeatUntilError});
    }

    return stop;
}

[[nodiscard]] auto TaskSeries::validateTaskData(const TaskData &task) const
    -> TaskData {
    validateDeadline(task.deadline);
    validateStart(task.start);

    return task;
}

TaskSeries::TaskSeries(const TaskData &task,
                       recurrencePattern_t recurrencePattern, stop_t stop)
    : task_(validateTaskData(task)),
      recurrencePattern_{std::move(recurrencePattern)},
      stop_{validateStop(stop)}, uuid_{core::uuid::generateUUID()} {}

auto TaskSeries::setStart(start_t start) -> void {
    task_.start = validateStart(start);
}

auto TaskSeries::setDeadline(deadline_t deadline) -> void {
    task_.deadline = validateDeadline(std::move(deadline));
}

auto TaskSeries::setRecurrencePattern(recurrencePattern_t recurrencePattern) {
    recurrencePattern_ = std::move(recurrencePattern);
}

auto TaskSeries::setStop(stop_t stop) -> void { stop_ = validateStop(stop); }

[[nodiscard]] auto TaskSeries::getRecurrencePattern() const
    -> recurrencePattern_t {
    return recurrencePattern_;
}

[[nodiscard]] auto TaskSeries::getStop() const -> stop_t { return stop_; }

[[nodiscard]] auto TaskSeries::isForDate(DateTime datetime) const -> bool {
    if (datetime < task_.start ||
        (stop_.has_value() && datetime > stop_.value())) {
        return false;
    }

    if (std::holds_alternative<util::IntervalRecurrence>(recurrencePattern_)) {
        return std::get<util::IntervalRecurrence>(recurrencePattern_)
            .happensOnDate(task_.start, datetime);
    }

    if (std::holds_alternative<util::WeekdayRecurrence>(recurrencePattern_)) {
        return std::get<util::WeekdayRecurrence>(recurrencePattern_)
            .happensOnDate(datetime);
    }

    return false;
}
} // namespace hbt::mods
