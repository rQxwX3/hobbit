#pragma once

#include <datetime/schema/datetime.hpp>
#include <datetime/schema/opt_datetime.hpp>
#include <recurrence/error/recurrence.hpp>
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
using ValidPatternEndDateTimeRelationship =
    Rule<[](const rec::Recurrence &recurrence) -> bool {
        const auto endDT{fields::EndDateTime::accessor(recurrence)};
        const auto patternType{std::visit(
            [](auto &&pattern) -> clndr::rec::pattern::Type {
                return pattern.getType();
            },
            fields::Pattern::accessor(recurrence))};

        return !endDT.hasValue() ||
               patternType != clndr::rec::pattern::Type::Null;
    },
         error::recurrence::InvalidPattern, fields::Pattern,
         fields::EndDateTime>;

using ValidStartDateTime =
    Rule<[](const rec::Recurrence &recurrence) -> bool {
        return dt::schema::datetime::Schema::checkAllRules(
            fields::StartDateTime::accessor(recurrence));
    },
         error::recurrence::InvalidStart, fields::StartDateTime>;

using ValidEndDateTime =
    Rule<[](const rec::Recurrence &recurrence) -> bool {
        return dt::schema::opt_datetime::Schema::checkAllRules(
            fields::EndDateTime::accessor(recurrence));
    },
         error::recurrence::InvalidEnd, fields::EndDateTime>;

using ValidStartEndRelationship =
    Rule<[](const rec::Recurrence &recurrence) -> bool {
        const auto startValue{fields::StartDateTime::accessor(recurrence)};
        const auto endValue{fields::EndDateTime::accessor(recurrence)};

        if (!endValue.hasValue()) {
            return true;
        }

        return startValue < endValue.getValue();
    },
         error::recurrence::InvalidStartEndRelationship, fields::StartDateTime,
         fields::EndDateTime>;

using all = Rules<ValidPatternEndDateTimeRelationship, ValidStartDateTime,
                  ValidEndDateTime, ValidStartEndRelationship>;
}; // namespace rules

using Schema = core::schema::Schema<rec::Recurrence, fields::all, rules::all>;
}; // namespace clndr::rec::schema::recurrence
