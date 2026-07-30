#pragma once

#include <algorithm>
#include <datetime/schema/date.hpp>
#include <datetime/week.hpp>
#include <schema/fields.hpp>
#include <schema/rules.hpp>
#include <schema/schema.hpp>

namespace clndr::dt::schema {
namespace fields {
using namespace core::schema::fields;
using Array = Field<dt::Week::array_t, dt::Week,
                    [](const dt::Week &week) -> dt::Week::array_t {
                        return week.toArray();
                    }>;

using all = Fields<Array>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidArray = Rule<[](const dt::Week &week) -> bool {
    const auto array{week.toArray()};

    return std::ranges::all_of(
               array,
               [](const dt::Date &date) -> bool {
                   return clndr::dt::schema::date::Schema::validate(date);
               }) &&
           std::ranges::is_sorted(array.begin(), array.end());
},
                        fields::Array>;

using all = Rules<ValidArray>;
}; // namespace rules

using Schema = core::schema::Schema<dt::Week, fields::all, rules::all>;
}; // namespace clndr::dt::schema
