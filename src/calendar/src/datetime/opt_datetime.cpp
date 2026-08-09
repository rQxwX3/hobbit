#include <datetime/error/opt_datetime.hpp>
#include <datetime/opt_datetime.hpp>
#include <datetime/schema/opt_datetime.hpp>

namespace clndr::dt {
OptDateTime::OptDateTime() = default;

OptDateTime::OptDateTime(value_t optDateTime) : value_{optDateTime} {
    schema::opt_datetime::Schema::validateAllRules(*this);
}

[[nodiscard]] auto OptDateTime::getValue() const -> DateTime { return *value_; }

auto OptDateTime::setValue(value_t value) -> void {
    value_ = value;

    schema::opt_datetime::Schema::validateAffectedRules<
        schema::opt_datetime::fields::Value>(*this);
}

[[nodiscard]] auto OptDateTime::hasValue() const -> bool {
    return value_.has_value();
}

[[nodiscard]] auto OptDateTime::getOptional() const -> value_t {
    return value_;
}
}; // namespace clndr::dt
