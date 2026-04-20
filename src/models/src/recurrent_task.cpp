#include <recurrent_task.hpp>

namespace hbt::mods {
[[nodiscard]] auto RecurrentTask::validateDeadline(deadline_t deadline)
    -> deadline_t {
    if (!std::holds_alternative<hbt::mods::Interval>(deadline)) {
        throw std::invalid_argument(std::string{invalidDeadlineError});
    }

    return deadline;
}

[[nodiscard]] auto
RecurrentTask::validateStartFrom(hbt::mods::DateTime startFrom)
    -> hbt::mods::DateTime {
    if (repeatUntil_.has_value() && startFrom > repeatUntil_.value()) {
        throw std::invalid_argument(std::string{invalideStartFromError});
    }

    return startFrom;
}

[[nodiscard]] auto RecurrentTask::validateRepeatUntil(repeatUntil_t repeatUntil)
    -> repeatUntil_t {
    if (repeatUntil.has_value() && repeatUntil.value() < startFrom_) {
        throw std::invalid_argument(std::string{invalideRepeatUntilError});
    }

    return repeatUntil;
}

RecurrentTask::RecurrentTask(std::string title, hbt::mods::DateTime startFrom,
                             recurrencePattern_t recurrencePattern,
                             repeatUntil_t repeatUntil, deadline_t deadline,
                             bool isCompleted)
    : Task(std::move(title), validateStartFrom(startFrom), isCompleted,
           validateDeadline(std::move(deadline))),
      recurrencePattern_{std::move(recurrencePattern)},
      repeatUntil_{validateRepeatUntil(repeatUntil)} {}

auto RecurrentTask::setStartFrom(hbt::mods::DateTime startFrom) -> void {
    startFrom_ = validateStartFrom(startFrom);
}

auto RecurrentTask::setDeadline(deadline_t deadline) -> void {
    deadline_ = validateDeadline(deadline);
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
    if (datetime < startFrom_ ||
        (repeatUntil_.has_value() && datetime > repeatUntil_.value())) {
        return false;
    }

    if (std::holds_alternative<util::IntervalRecurrence>(recurrencePattern_)) {
        return std::get<util::IntervalRecurrence>(recurrencePattern_)
            .happensOnDate(startFrom_, datetime);
    }

    if (std::holds_alternative<util::WeekdayRecurrence>(recurrencePattern_)) {
        return std::get<util::WeekdayRecurrence>(recurrencePattern_)
            .happensOnDate(datetime);
    }

    return false;
}
} // namespace hbt::mods
