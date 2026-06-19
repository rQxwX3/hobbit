#pragma once

#include <recurrence_pattern.hpp>

namespace hbt::mods::util {
class NullRecurrencePattern : RecurrencePattern {
  private:
    DateTime dateTime_;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime on) const
        -> occurrences_t override;
};
} // namespace hbt::mods::util
