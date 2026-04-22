#pragma once

#include <date.hpp>
#include <interval.hpp>

#include <nlohmann/json.hpp>

#include <chrono>

namespace hbt::mods {

class DateTime {
  public:
    using time_t = std::chrono::minutes;

    using year_t = Date::year_t;
    using month_t = Date::month_t;
    using day_t = Date::day_t;
    using weekday_t = Date::weekday_t;

    using hour_t = std::chrono::hours;
    using minute_t = std::chrono::minutes;

  public:
    static constexpr time_t timeInMinute{1};
    static constexpr time_t timeInHour{timeInMinute *
                                       DurationUnits::minutesInHour};
    static constexpr time_t timeInDay{timeInHour * DurationUnits::hoursInDay};

  private:
    mods::Date date_;
    time_t time_;

  public:
    DateTime();

    explicit DateTime(mods::Date date, time_t time = time_t{0});

    explicit DateTime(year_t year, month_t month, day_t day,
                      hour_t hour = hour_t{0}, minute_t minute = minute_t{0});

  public:
    [[nodiscard]] static auto now() -> DateTime;

  public:
    [[nodiscard]] auto getDate() const -> mods::Date;

  public:
    [[nodiscard]] auto getTime() const -> time_t;

    [[nodiscard]] auto getHour() const -> hour_t;

    [[nodiscard]] auto getMinute() const -> minute_t;

  public:
    [[nodiscard]] static auto equalDates(DateTime dt1, DateTime dt2) -> bool;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::optional<DateTime>;

  public:
    [[nodiscard]] auto operator<=>(const DateTime &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const DateTime &other) const -> bool;

    [[nodiscard]] auto operator+(const Interval &interval) const -> DateTime;

    auto operator+=(const Interval &interval) -> DateTime &;

    [[nodiscard]] auto operator-(const DateTime &other) const -> DurationUnits;
};
} // namespace hbt::mods
