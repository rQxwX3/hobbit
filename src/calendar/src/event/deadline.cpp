#include <event/deadline.hpp>
#include <event/error/deadline.hpp>
#include <event/schema/deadline.hpp>

namespace clndr::ev {
Deadline::Deadline() = default;

Deadline::Deadline(underlying_type_t type) : type_{type} {}

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

    std::unreachable();
}

[[nodiscard]] auto Deadline::getInterval() const -> dt::Interval {
    return std::get<dt::Interval>(type_);
}

[[nodiscard]] auto Deadline::getDateTime() const -> dt::DateTime {
    return std::get<dt::DateTime>(type_);
}

[[nodiscard]] auto Deadline::operator==(const Deadline &deadline) const
    -> bool = default;
} // namespace clndr::ev
