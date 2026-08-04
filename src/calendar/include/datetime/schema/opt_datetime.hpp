#pragma once

#include <datetime/opt_datetime.hpp>
#include <datetime/schema/datetime.hpp>
#include <schema/schema.hpp>

namespace clndr::dt::schema::opt_datetime {
namespace fields {
using namespace core::schema::fields;
using Value =
    Field<dt::OptDateTime::value_t,
          [](const dt::OptDateTime &opt_datetime) -> dt::OptDateTime::value_t {
              return opt_datetime.getValue();
          }>;

using all = Fields<Value>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidValue = Rule<[](const dt::OptDateTime &opt_datetime) -> bool {
    const auto value{fields::Value::accessor(opt_datetime)};

    if (!value.has_value()) {
        return true;
    }

    return schema::datetime::Schema::validate(value.value());
},
                        fields::Value>;

using all = Rules<ValidValue>;
}; // namespace rules

using Schema = core::schema::Schema<dt::OptDateTime, fields::all, rules::all>;
}; // namespace clndr::dt::schema::opt_datetime
