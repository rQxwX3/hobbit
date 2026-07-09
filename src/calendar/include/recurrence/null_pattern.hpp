#pragma once

#include <recurrence/pattern.hpp>

#include <nlohmann/json.hpp>

namespace clndr::rec {
class NullPattern : public Pattern {
  public:
    NullPattern() = default;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(dt::Date date,
                                            dt::DateTime start) const
        -> occurrences_t override;

    [[nodiscard]] auto happensOnDate(dt::Date date, dt::DateTime start) const
        -> bool override;
};
} // namespace clndr::rec
