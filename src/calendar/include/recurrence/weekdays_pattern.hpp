#pragma once

#include <datetime/week.hpp>
#include <recurrence/pattern.hpp>
#include <recurrence/selected_weekdays.hpp>

namespace clndr::rec {
class WeekdaysPattern : public Pattern {
  private:
    /* don't change the order */
    dt::Week firstWeek_;
    dt::Interval interval_;
    SelectedWeekdays selectedWeekdays_;

  public:
    WeekdaysPattern(dt::Date startDate, rec::SelectedWeekdays selectedWDs,
                    dt::Interval interval);

    WeekdaysPattern(dt::Week firstWeek, rec::SelectedWeekdays selectedWDs,
                    dt::Interval interval);

  public:
    [[nodiscard]] auto ok() const -> bool;

    template <typename Field> [[nodiscard]] auto fieldOK() const -> bool;

  public:
    [[nodiscard]] auto
    getOccurrencesOfDate(dt::Date date,
                         dt::DateTime start = dt::DateTime::now()) const
        -> occurrences_t override;

    [[nodiscard]] auto
    happensOnDate(dt::Date date, dt::DateTime start = dt::DateTime::now()) const
        -> bool override;

  private:
    [[nodiscard]] static auto getFirstOccurrence(dt::Date startDate,
                                                 SelectedWeekdays selectedWDs)
        -> dt::DateTime;

  public:
    [[nodiscard]] auto getInterval() const -> dt::Interval;

    [[nodiscard]] auto getFirstWeek() const -> dt::Week;

    [[nodiscard]] auto getSelectedWeekdays() const -> SelectedWeekdays;
};
} // namespace clndr::rec
