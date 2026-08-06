#pragma once

#include <recurrence/pattern.hpp>

namespace clndr::rec {
class IntervalPattern : public Pattern {
  private:
    dt::Interval interval_;

  public:
    IntervalPattern(dt::Interval interval);

  public:
    [[nodiscard]] auto ok() const -> bool;

    template <typename Field> [[nodiscard]] auto fieldOK() const -> bool;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(dt::Date date,
                                            dt::DateTime start) const
        -> occurrences_t override;

    [[nodiscard]] auto happensOnDate(dt::Date date, dt::DateTime start) const
        -> bool override;

  public:
    [[nodiscard]] auto getInterval() const -> dt::Interval;

  public:
    [[nodiscard]] auto getFirstOccurrenceOfDate(dt::Date date,
                                                dt::DateTime start) const
        -> std::optional<occurrence_t>;

  public:
    [[nodiscard]] auto operator==(const IntervalPattern &other) const -> bool;
};
} // namespace clndr::rec
