#include <recurrent_task.hpp>

namespace hbt::mods {
RecurrentTask::RecurrentTask(std::string title, hbt::mods::DateTime startFrom,
                             recurrencePattern_t recurrencePattern,
                             repeatUntil_t repeatUntil, deadline_t deadline,
                             bool isCompleted)
    : Task(std::move(title), startFrom, deadline, isCompleted),
      recurrencePattern_{std::move(recurrencePattern)},
      repeatUntil_{repeatUntil} {}

auto RecurrentTask::setRecurrencePattern(
    recurrencePattern_t recurrencePattern) {
    recurrencePattern_ = recurrencePattern;
}

auto RecurrentTask::setRepeatUntil(repeatUntil_t repeatUntil) {
    if (repeatUntil < hbt::mods::DateTime::now()) {
        // TODO: handle error
        return;
    }

    repeatUntil_ = repeatUntil;
}

[[nodiscard]] auto RecurrentTask::getRecurrencePattern() const
    -> recurrencePattern_t {
    return recurrencePattern_;
}

[[nodiscard]] auto RecurrentTask::getRepeatUntil() const -> repeatUntil_t {
    return repeatUntil_;
}
} // namespace hbt::mods
