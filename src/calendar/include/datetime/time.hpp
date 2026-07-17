#pragma once

#include <chrono>
#include <cstdint>

#include <datetime/interval.hpp>

namespace clndr::dt {
class Time {
  public:
    using hour_t = uint8_t;
    using minute_t = uint8_t;

    using duration_t = std::chrono::minutes;

  private:
    /* order must not be changed */
    hour_t hour_;
    minute_t minute_;

  public:
    Time(hour_t hour, minute_t minute);

  public:
    [[nodiscard]] auto getHour() const -> hour_t;

    [[nodiscard]] auto getMinute() const -> minute_t;

  public:
    [[nodiscard]] auto toDuration() const -> duration_t;

    [[nodiscard]] auto operator<=>(const Time &other) const
        -> std::strong_ordering = default;
};
} // namespace clndr::dt
