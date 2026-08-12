#pragma once

#include <datetime/week.hpp>
#include <recurrence/pattern.hpp>
#include <recurrence/selected_weekdays.hpp>

namespace clndr::rec {
class WeekdaysPattern : public pattern::Base {
  public:
    static constexpr auto type{pattern::Type::Weekdays};

  private:
    /* don't change the order */
    dt::Week firstWeek_;
    dt::Interval interval_;
    SelectedWeekdays selectedWeekdays_;

  public:
    WeekdaysPattern();

    WeekdaysPattern(dt::Date startDate, rec::SelectedWeekdays selectedWDs,
                    dt::Interval interval);

    WeekdaysPattern(dt::Week firstWeek, rec::SelectedWeekdays selectedWDs,
                    dt::Interval interval);

  public:
    [[nodiscard]] auto getType() const -> pattern::Type override;

    [[nodiscard]] auto getInterval() const -> dt::Interval;

    [[nodiscard]] auto getFirstWeek() const -> dt::Week;

    [[nodiscard]] auto getSelectedWeekdays() const -> SelectedWeekdays;

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
};

static_assert(pattern::Concept<WeekdaysPattern>);
} // namespace clndr::rec
