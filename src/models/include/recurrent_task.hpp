#pragma once

#include <datetime.hpp>
#include <recurrence_patterns.hpp>
#include <task.hpp>

#include <optional>
#include <variant>

namespace hbt::mods {

class RecurrentTask : public Task {
  public:
    using repeatUntil_t = std::optional<hbt::mods::DateTime>;
    using recurrencePattern_t =
        std::variant<hbt::mods::util::IntervalRecurrence,
                     hbt::mods::util::WeekdayRecurrence>;

  private:
    recurrencePattern_t recurrencePattern_;

    repeatUntil_t repeatUntil_{std::nullopt};

  public:
    RecurrentTask(std::string title, hbt::mods::DateTime startFrom,
                  recurrencePattern_t recurrencePattern,
                  repeatUntil_t repeatUntil = std::nullopt,
                  deadline_t deadline = std::nullopt, bool isCompleted = false);

  public:
    auto setRecurrencePattern(recurrencePattern_t recurrencePattern);

    auto setRepeatUntil(repeatUntil_t repeatUntil);

  public:
    [[nodiscard]] auto getRecurrencePattern() const -> recurrencePattern_t;

    [[nodiscard]] auto getRepeatUntil() const -> repeatUntil_t;

  public:
    [[nodiscard]] auto isForDate(hbt::mods::DateTime datetime) const -> bool;
};
} // namespace hbt::mods
