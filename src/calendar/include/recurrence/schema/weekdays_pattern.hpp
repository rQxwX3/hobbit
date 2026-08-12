#pragma once

#include <datetime/schema/interval.hpp>
#include <datetime/schema/week.hpp>
#include <recurrence/error/weekdays_pattern.hpp>
#include <recurrence/weekdays_pattern.hpp>
#include <schema/fields.hpp>
#include <schema/rules.hpp>
#include <schema/schema.hpp>

namespace clndr::rec::schema::weekdays_pattern {
namespace fields {
using namespace core::schema::fields;
using FirstWeek =
    Field<dt::Week,
          [](const rec::WeekdaysPattern &weekdaysPattern) -> dt::Week {
              return weekdaysPattern.getFirstWeek();
          }>;

using Interval =
    Field<dt::Interval,
          [](const rec::WeekdaysPattern &weekdaysPattern) -> dt::Interval {
              return weekdaysPattern.getInterval();
          }>;

using SelectedWeekdays = Field<rec::SelectedWeekdays,
                               [](const rec::WeekdaysPattern &weekdaysPattern)
                                   -> rec::SelectedWeekdays {
                                   return weekdaysPattern.getSelectedWeekdays();
                               }>;
using all = Fields<FirstWeek, Interval, SelectedWeekdays>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidInterval =
    Rule<[](const rec::WeekdaysPattern &weekdaysPattern) -> bool {
        return fields::Interval::accessor(weekdaysPattern)
            .onlyContainsUnit(dt::Interval::WEEK);
    },
         error::weekdays_pattern::InvalidInterval, fields::Interval>;

using ValidSelectedWeekdays =
    Rule<[](const rec::WeekdaysPattern &weekdaysPattern) -> bool {
        return !fields::SelectedWeekdays::accessor(weekdaysPattern).isEmpty();
    },
         error::weekdays_pattern::InvalidSelectedWeekdays,
         fields::SelectedWeekdays>;

using all = Rules<ValidInterval, ValidSelectedWeekdays>;
}; // namespace rules

using Schema =
    core::schema::Schema<rec::WeekdaysPattern, fields::all, rules::all>;
}; // namespace clndr::rec::schema::weekdays_pattern
