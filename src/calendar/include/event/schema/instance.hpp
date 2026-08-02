#pragma once

#include <datetime/schema/datetime.hpp>
#include <event/instance.hpp>
#include <schema/fields.hpp>
#include <schema/rules.hpp>
#include <schema/schema.hpp>

namespace clndr::ev::schema::instance {
namespace fields {
using namespace core::schema::fields;

using DateTime =
    Field<dt::DateTime, [](const ev::Instance &instance) -> dt::DateTime {
        return instance.getDateTime();
    }>;

using all = Fields<DateTime>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;

using ValidDateTime = Rule<[](const ev::Instance &instance) -> bool {
    return dt::schema::datetime::Schema::validate(
        fields::DateTime::accessor(instance));
},
                           fields::DateTime>;

using all = Rules<ValidDateTime>;
}; // namespace rules

using Schema = core::schema::Schema<ev::Instance, fields::all, rules::all>;
}; // namespace clndr::ev::schema::instance
