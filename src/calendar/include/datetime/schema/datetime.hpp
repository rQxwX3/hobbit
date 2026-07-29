#pragma once

#include <datetime/datetime.hpp>
#include <datetime/schema/date.hpp>
#include <datetime/schema/time.hpp>
#include <schema/fields.hpp>
#include <schema/rules.hpp>
#include <schema/schema.hpp>

namespace clndr::dt::schema::datetime {
namespace fields {
using namespace core::schema::fields;
using Date =
    Field<dt::Date, dt::DateTime, [](const dt::DateTime &datetime) -> dt::Date {
        return datetime.getDate();
    }>;

using Time =
    Field<dt::Time, dt::DateTime, [](const dt::DateTime &datetime) -> dt::Time {
        return datetime.getTime();
    }>;

using all = Fields<Date, Time>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidDate = Rule<[](const dt::DateTime &datetime) -> bool {
    return dt::schema::date::Schema::validate(fields::Date::accessor(datetime));
},
                       fields::Date>;

using ValidTime = Rule<[](const dt::DateTime &datetime) -> bool {
    return dt::schema::time::Schema::validate(fields::Time::accessor(datetime));
},
                       fields::Time>;

using all = Rules<ValidDate, ValidTime>;
}; // namespace rules

using Schema = core::schema::Schema<dt::DateTime, fields::all, rules::all>;
}; // namespace clndr::dt::schema::datetime
