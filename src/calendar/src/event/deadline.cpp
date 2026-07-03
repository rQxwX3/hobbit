#include <event/deadline.hpp>

namespace clndr::ev {
Deadline::Deadline(underlying_type_t type)
    : type_{validateUnderlyingType(std::move(type))} {}

[[nodiscard]] auto Deadline::validateUnderlyingType(underlying_type_t type)
    -> underlying_type_t {
    if (std::holds_alternative<dt::Interval>(type) ||
        std::holds_alternative<dt::DateTime>(type) ||
        std::holds_alternative<std::monostate>(type)) {
        return type;
    }

    throw std::invalid_argument(errorMessage(Error::InvalidUnderlyingType));
}

[[nodiscard]] auto Deadline::null() -> Deadline { return {std::monostate()}; }

[[nodiscard]] auto Deadline::isNull() const -> bool {
    return std::holds_alternative<std::monostate>(type_);
}

[[nodiscard]] auto Deadline::isInterval() const -> bool {
    return std::holds_alternative<dt::Interval>(type_);
}

[[nodiscard]] auto Deadline::isDateTime() const -> bool {
    return std::holds_alternative<dt::DateTime>(type_);
}

[[nodiscard]] auto Deadline::getUnderlyingType() const -> underlying_type_t {
    return type_;
}

[[nodiscard]] auto Deadline::getType() const -> Type {
    if (std::holds_alternative<dt::Interval>(type_)) {
        return Type::Interval;
    }

    if (std::holds_alternative<dt::DateTime>(type_)) {
        return Type::DateTime;
    }

    if (std::holds_alternative<std::monostate>(type_)) {
        return Type::Null;
    }

    throw std::runtime_error(errorMessage(Error::RTInvalidUnderlyingType));
}

[[nodiscard]] auto Deadline::getInterval() const -> dt::Interval {
    if (getType() != Type::Interval) {
        throw std::runtime_error(errorMessage(Error::IntervalBadAccess));
    }

    return std::get<dt::Interval>(type_);
}

[[nodiscard]] auto Deadline::getDateTime() const -> dt::DateTime {
    if (getType() != Type::DateTime) {
        throw std::runtime_error(errorMessage(Error::DateTimeBadAccess));
    }

    return std::get<dt::DateTime>(type_);
}

[[nodiscard]] auto Deadline::operator==(const Deadline &deadline) const
    -> bool = default;
} // namespace clndr::ev
