#pragma once

#include <recurrence/pattern.hpp>

namespace clndr::rec {
class IntervalPattern : public pattern::Base {
  public:
    static constexpr auto type{pattern::Type::Interval};

  private:
    dt::Interval interval_;

  public:
    IntervalPattern();

    IntervalPattern(dt::Interval interval);

  public:
    [[nodiscard]] auto getType() const -> pattern::Type override;

    [[nodiscard]] auto getInterval() const -> dt::Interval;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(dt::Date date,
                                            dt::DateTime start) const
        -> occurrences_t override;

    [[nodiscard]] auto happensOnDate(dt::Date date, dt::DateTime start) const
        -> bool override;

  public:
    [[nodiscard]] auto getFirstOccurrenceOfDate(dt::Date date,
                                                dt::DateTime start) const
        -> std::optional<occurrence_t>;

  public:
    [[nodiscard]] auto operator==(const IntervalPattern &other) const -> bool;
};

static_assert(pattern::Concept<IntervalPattern>);
} // namespace clndr::rec
