#include <codec/json/interval.hpp>
#include <codec/json/week.hpp>
#include <recurrence/weekdays_pattern.hpp>

namespace clndr::rec {
auto WeekdaysPattern::validateInterval(const dt::Interval &interval)
    -> dt::Interval {
    if (!interval.onlyContainsUnit(dt::Interval::Unit::WEEK)) {
        throw std::invalid_argument(
            Error::getMessageForCode(Error::Code::InvalidInterval));
    }

    return interval;
}

auto WeekdaysPattern::validateSelectedWeekdays(
    const rec::SelectedWeekdays &selectedWDs) -> rec::SelectedWeekdays {
    if (selectedWDs.isEmpty()) {
        throw std::invalid_argument(
            Error::getMessageForCode(Error::Code::EmptyWeek));
    }

    return selectedWDs;
}
WeekdaysPattern::WeekdaysPattern(dt::Date startDate,
                                 rec::SelectedWeekdays selectedWDs,
                                 dt::Interval interval)
    : firstWeek_{dt::Week(
          getFirstOccurrence(startDate, validateSelectedWeekdays(selectedWDs))
              .getDate())},
      interval_{validateInterval(interval)}, selectedWeekdays_{selectedWDs} {}

WeekdaysPattern::WeekdaysPattern(dt::Week firstWeek,
                                 rec::SelectedWeekdays selectedWDs,
                                 dt::Interval interval)
    : firstWeek_{firstWeek}, interval_{validateInterval(interval)},
      selectedWeekdays_{validateSelectedWeekdays(selectedWDs)} {}

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
