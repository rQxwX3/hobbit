#pragma once

#include <recurrence_pattern.hpp>

namespace hbt::mods::util {
class NullRecurrencePattern : public RecurrencePattern {
  public:
    NullRecurrencePattern() = default;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime on) const
        -> occurrences_t override;
};
} // namespace hbt::mods::util
