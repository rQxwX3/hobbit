#include <datetime/error/time.hpp>
#include <datetime/schema/time.hpp>
#include <datetime/time.hpp>

namespace clndr::dt {
Time::Time() = default;

Time::Time(hour_t hour, minute_t minute) : hour_{hour}, minute_{minute} {
    schema::time::Schema::validateAllRules(*this);
}

[[nodiscard]] auto Time::getHour() const -> hour_t { return hour_; }

[[nodiscard]] auto Time::getMinute() const -> minute_t { return minute_; }

auto Time::setHour(hour_t hour) -> void {
    hour_ = hour;

    schema::time::Schema::validateAffectedRules<schema::time::fields::Hour>(
        *this);
}

auto Time::setMinute(minute_t minute) -> void {
    minute_ = minute;

    schema::time::Schema::validateAffectedRules<schema::time::fields::Minute>(
        *this);
}

[[nodiscard]] auto Time::midnight() -> Time { return {0, 0}; }

[[nodiscard]] auto Time::toDuration() const -> duration_t {
    return duration_t((hour_ * constants::minutesInHour) + minute_);
}
} // namespace clndr::dt
