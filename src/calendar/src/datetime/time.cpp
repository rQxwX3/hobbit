#include <datetime/error/time.hpp>
#include <datetime/schema/time.hpp>
#include <datetime/time.hpp>

namespace clndr::dt {
Time::Time(hour_t hour, minute_t minute) : hour_{hour}, minute_{minute} {
    if (!ok()) {
        throw std::invalid_argument(
            std::string(error::time::InvalidCtorArgs::msg));
    }
}

[[nodiscard]] auto Time::midnight() -> Time { return {0, 0}; }

[[nodiscard]] auto Time::ok() const -> bool {
    return schema::time::Schema::validate(*this);
}

[[nodiscard]] auto Time::getHour() const -> hour_t { return hour_; }

[[nodiscard]] auto Time::getMinute() const -> minute_t { return minute_; }

[[nodiscard]] auto Time::toDuration() const -> duration_t {
    return duration_t((hour_ * constants::minutesInHour) + minute_);
}
} // namespace clndr::dt
