#pragma once

#include <datetime/schema/datetime.hpp>
#include <datetime/schema/opt_datetime.hpp>
#include <recurrence/recurrence.hpp>
#include <schema/schema.hpp>

namespace clndr::rec::schema::recurrence {
namespace fields {
using namespace core::schema::fields;
using Pattern =
    Field<rec::Recurrence::pattern_t,
          [](const rec::Recurrence &recurrence) -> rec::Recurrence::pattern_t {
              return recurrence.getPattern();
          }>;

using StartDateTime =
    Field<dt::DateTime, [](const rec::Recurrence &recurrence) -> dt::DateTime {
        return recurrence.getStartDateTime();
    }>;

using EndDateTime =
    Field<dt::OptDateTime,
          [](const rec::Recurrence &recurrence) -> dt::OptDateTime {
              return recurrence.getEndDateTime();
          }>;

using all = Fields<Pattern, StartDateTime, EndDateTime>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidPattern = Rule<[](const rec::Recurrence &recurrence) -> bool {
    const auto value{fields::Pattern::accessor(recurrence)};

    return std::visit([](auto &pattern) -> bool { return pattern.ok(); },
                      value);
},
                          fields::Pattern>;

using ValidStartDateTime = Rule<[](const rec::Recurrence &recurrence) -> bool {
    return dt::schema::datetime::Schema::validate(
        fields::StartDateTime::accessor(recurrence));
},
                                fields::StartDateTime>;

using ValidEndDateTime = Rule<[](const rec::Recurrence &recurrence) -> bool {
    return dt::schema::opt_datetime::Schema::validate(
        fields::EndDateTime::accessor(recurrence));
},
                              fields::EndDateTime>;
using ValidStartEndRelation =
    Rule<[](const rec::Recurrence &recurrence) -> bool {
        const auto startValue{fields::StartDateTime::accessor(recurrence)};
        const auto endValue{fields::EndDateTime::accessor(recurrence)};

        if (!endValue.hasValue()) {
            return true;
        }

        return startValue < endValue.getValue();
    },
         fields::StartDateTime, fields::EndDateTime>;

using all = Rules<ValidPattern, ValidStartDateTime, ValidEndDateTime,
                  ValidStartEndRelation>;
}; // namespace rules

using Schema = core::schema::Schema<rec::Recurrence, fields::all, rules::all>;
}; // namespace clndr::rec::schema::recurrence
