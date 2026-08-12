#pragma once

#include <event/deadline.hpp>
#include <schema/fields.hpp>
#include <schema/rules.hpp>
#include <schema/schema.hpp>

namespace clndr::ev::schema::deadline {
namespace fields {
using namespace core::schema::fields;
using UnderlyingType =
    Field<ev::Deadline::underlying_type_t,
          [](const ev::Deadline &deadline) -> ev::Deadline::underlying_type_t {
              return deadline.getUnderlyingType();
          }>;

using all = Fields<UnderlyingType>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;

using all = Rules<>;
}; // namespace rules

using Schema = core::schema::Schema<ev::Deadline, fields::all, rules::all>;
}; // namespace clndr::ev::schema::deadline
