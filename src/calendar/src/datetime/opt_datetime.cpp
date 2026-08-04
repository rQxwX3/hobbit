#include <datetime/opt_datetime.hpp>
#include <datetime/schema/opt_datetime.hpp>

namespace clndr::dt {
OptDateTime::OptDateTime(std::optional<DateTime> optDateTime)
    : value_{optDateTime} {
    if (!ok()) {
        throw std::runtime_error(
            Error::getMessage(Error::Code::InvalidCtorArgs));
    }
}

[[nodiscard]] auto OptDateTime::ok() const -> bool {
    return schema::opt_datetime::Schema::validate(*this);
}

[[nodiscard]] auto OptDateTime::hasValue() const -> bool {
    return value_.has_value();
}

[[nodiscard]] auto OptDateTime::getValue() const -> DateTime { return *value_; }

[[nodiscard]] auto OptDateTime::getOptional() const -> value_t {
    return value_;
}
}; // namespace clndr::dt
