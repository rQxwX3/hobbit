#include <datetime/time.hpp>
#include <schema/time.hpp>

namespace clndr::dt {
Time::Time(hour_t hour, minute_t minute) : hour_{hour}, minute_{minute} {}

[[nodiscard]] auto Time::getHour() const -> hour_t { return hour_; }

[[nodiscard]] auto Time::getMinute() const -> minute_t { return minute_; }

[[nodiscard]] auto Time::toDuration() const -> duration_t {
    return duration_t((hour_ * Interval::minutesInHour) + minute_);
}
} // namespace clndr::dt
