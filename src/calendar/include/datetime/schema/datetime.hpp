#pragma once

#include <datetime/datetime.hpp>
#include <datetime/error/datetime.hpp>
#include <schema/schema.hpp>

namespace clndr::dt::schema::datetime {
namespace fields {
using namespace core::schema::fields;
using Value = Field<dt::DateTime::value_t,
                    [](const dt::DateTime &datetime) -> dt::DateTime::value_t {
                        return datetime.getValue();
                    }>;

using all = Fields<Value>;
}; // namespace fields

namespace rules {
using namespace core::schema::rules;
using ValidValue = Rule<[](const dt::DateTime &datetime) -> bool {
    const auto value{fields::Value::accessor(datetime)};

    return (std::numeric_limits<dt::DateTime::value_t>::min() <= value) &&
           (value <= std::numeric_limits<dt::DateTime::value_t>::max());
},
                        error::datetime::InvalidValue, fields::Value>;

using all = Rules<ValidValue>;
}; // namespace rules

using Schema = core::schema::Schema<dt::DateTime, fields::all, rules::all>;
}; // namespace clndr::dt::schema::datetime
