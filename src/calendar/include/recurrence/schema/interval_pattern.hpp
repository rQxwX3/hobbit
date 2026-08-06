#pragma once

#include <datetime/schema/interval.hpp>
#include <recurrence/interval_pattern.hpp>
#include <schema/schema.hpp>

namespace clndr::rec::schema::interval_pattern {
namespace fields {
using namespace core::schema::fields;
using Interval =
    Field<dt::Interval,
          [](const rec::IntervalPattern &intervalPattern) -> dt::Interval {
              return intervalPattern.getInterval();
          }>;

using all = Fields<Interval>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidInterval =
    Rule<[](const rec::IntervalPattern &intervalPattern) -> bool {
        const auto value{fields::Interval::accessor(intervalPattern)};

        return dt::schema::interval::Schema::validate(value) && !value.isZero();
    },
         fields::Interval>;

using all = Rules<ValidInterval>;
}; // namespace rules

using Schema =
    core::schema::Schema<rec::IntervalPattern, fields::all, rules::all>;
}; // namespace clndr::rec::schema::interval_pattern
