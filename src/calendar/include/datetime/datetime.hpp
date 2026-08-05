#pragma once

#include <chrono>

#include <nlohmann/json.hpp>

#include <datetime/constants.hpp>
#include <datetime/date.hpp>
#include <datetime/interval.hpp>
#include <datetime/time.hpp>

namespace clndr::dt {
class DateTime {
  public:
    using duration_t = std::chrono::minutes;
    using value_t = std::chrono::sys_time<duration_t>;

  private:
    value_t value_;

  public:
    [[nodiscard]] auto ok() const -> bool;

  private:
    [[nodiscard]] auto getDaysSinceEpoch() const -> Date::duration_t;

    [[nodiscard]] auto getMinutesSinceMidnight() const -> Time::duration_t;

  public:
    DateTime();

    DateTime(value_t value);

    DateTime(Date date, Time time = Time::midnight());

  public:
    [[nodiscard]] static auto now() -> DateTime;

  public:
    [[nodiscard]] auto getDate() const -> Date;

    [[nodiscard]] auto getTime() const -> Time;

    [[nodiscard]] auto getWeekday() const -> constants::Weekday;

    [[nodiscard]] auto getValue() const -> value_t;

  public:
    [[nodiscard]] static auto equalDate(DateTime dt1, DateTime dt2) -> bool;

    [[nodiscard]] static auto equalTime(DateTime dt1, DateTime dt2) -> bool;

  public:
    [[nodiscard]] auto operator<=>(const DateTime &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const DateTime &other) const -> bool;

    [[nodiscard]] auto operator+(const Interval &interval) const -> DateTime;

    auto operator+=(const Interval &interval) -> DateTime &;

  public:
    [[nodiscard]] static auto diff(const DateTime &dt1, const DateTime &dt2)
        -> Interval;

    [[nodiscard]] static auto daysDiff(const DateTime &dt1, const DateTime &dt2)
        -> Interval;
};
} // namespace clndr::dt
