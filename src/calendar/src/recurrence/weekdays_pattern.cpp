#include <recurrence/error/weekdays_pattern.hpp>
#include <recurrence/schema/weekdays_pattern.hpp>
#include <recurrence/weekdays_pattern.hpp>

namespace clndr::rec {
WeekdaysPattern::WeekdaysPattern() = default;

WeekdaysPattern::WeekdaysPattern(dt::Date startDate,
                                 rec::SelectedWeekdays selectedWDs,
                                 dt::Interval interval)
    : firstWeek_{dt::Week(
          getFirstOccurrence(startDate, selectedWDs).getDate())},
      interval_{interval}, selectedWeekdays_{selectedWDs} {
    schema::weekdays_pattern::Schema::validateAllRules(*this);
}

WeekdaysPattern::WeekdaysPattern(dt::Week firstWeek,
                                 rec::SelectedWeekdays selectedWDs,
                                 dt::Interval interval)
    : firstWeek_{firstWeek}, interval_{interval},
      selectedWeekdays_{selectedWDs} {
    schema::weekdays_pattern::Schema::validateAllRules(*this);
}

[[nodiscard]] auto WeekdaysPattern::getType() const -> pattern::Type {
    return type;
}

[[nodiscard]] auto WeekdaysPattern::getInterval() const -> dt::Interval {
    return interval_;
}

[[nodiscard]] auto WeekdaysPattern::getFirstWeek() const -> dt::Week {
    return firstWeek_;
}

[[nodiscard]] auto WeekdaysPattern::getSelectedWeekdays() const
    -> SelectedWeekdays {
    return selectedWeekdays_;
}

[[nodiscard]] auto WeekdaysPattern::getFirstOccurrence(
    dt::Date startDate, rec::SelectedWeekdays selectedWDs) -> dt::DateTime {
    assert(!selectedWDs.isEmpty());

    auto result{startDate};

    while (!selectedWDs.contains(result.getWeekday())) {
        result += dt::Interval::days(1);
    }

    return result;
}

[[nodiscard]] auto
WeekdaysPattern::happensOnDate(dt::Date date, dt::DateTime startDateTime) const
    -> bool {
    const auto dateWD{date.getWeekday()};

    if (!selectedWeekdays_.contains(dateWD)) {
        return false;
    }

    auto firstInstanceOfWeekday(firstWeek_[dateWD]);
    auto daysDiff{dt::DateTime::daysDiff(firstInstanceOfWeekday, date)};

    return daysDiff.isZero() ||
           (date > firstInstanceOfWeekday && daysDiff.isMultipleOf(interval_));
}

[[nodiscard]] auto
WeekdaysPattern::getOccurrencesOfDate(dt::Date date, dt::DateTime start) const
    -> occurrences_t {
    if (happensOnDate(date)) {
        return {dt::DateTime(date)};
    }

    return {};
}
} // namespace clndr::rec
