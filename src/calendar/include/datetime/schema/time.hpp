#pragma once

#include <datetime/constants.hpp>
#include <datetime/time.hpp>
#include <schema/fields.hpp>
#include <schema/rules.hpp>
#include <schema/schema.hpp>

namespace clndr::dt::schema::time {
namespace fields {
using namespace core::schema::fields;
using Hour =
    Field<dt::Time::hour_t, [](const dt::Time &time) -> dt::Time::hour_t {
        return time.getHour();
    }>;

using Minute =
    Field<dt::Time::minute_t, [](const dt::Time &time) -> dt::Time::minute_t {
        return time.getMinute();
    }>;

using all = Fields<Hour, Minute>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidHour = Rule<[](const dt::Time &time) -> bool {
    const auto value{fields::Hour::accessor(time)};

    return (dt::constants::minHourValue <= value) &&
           (value <= dt::constants::maxHourValue);
},
                       fields::Hour>;

using ValidMinute = Rule<[](const dt::Time &time) -> bool {
    const auto value{fields::Minute::accessor(time)};

    return (dt::constants::minMinuteValue <= value) &&
           (value <= dt::constants::maxMinuteValue);
},
                         fields::Minute>;

using all = Rules<ValidHour, ValidMinute>;
}; // namespace rules

using Schema = core::schema::Schema<dt::Time, fields::all, rules::all>;
}; // namespace clndr::dt::schema::time
