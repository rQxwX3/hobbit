#pragma once

#include <datetime/schema/interval.hpp>
#include <datetime/schema/week.hpp>
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
using ValidFirstWeek =
    Rule<[](const rec::WeekdaysPattern &weekdaysPattern) -> bool {
        return dt::schema::week::Schema::validate(
            fields::FirstWeek::accessor(weekdaysPattern));
    },
         fields::FirstWeek>;

using ValidInterval =
    Rule<[](const rec::WeekdaysPattern &weekdaysPattern) -> bool {
        const auto value{fields::Interval::accessor(weekdaysPattern)};

        if (!dt::schema::interval::Schema::validate(value)) {
            return false;
        }

        return value.onlyContainsUnit(dt::Interval::WEEK);
    },
         fields::Interval>;

using ValidSelectedWeekdays =
    Rule<[](const rec::WeekdaysPattern &weekdaysPattern) -> bool {
        return !fields::SelectedWeekdays::accessor(weekdaysPattern).isEmpty();
    },
         fields::SelectedWeekdays>;

using all = Rules<ValidFirstWeek, ValidInterval, ValidSelectedWeekdays>;
}; // namespace rules

using Schema =
    core::schema::Schema<rec::WeekdaysPattern, fields::all, rules::all>;
}; // namespace clndr::rec::schema::weekdays_pattern
