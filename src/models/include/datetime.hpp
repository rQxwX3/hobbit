#pragma once

#include <nlohmann/json.hpp>

#include <interval.hpp>

#include <chrono>

namespace hbt::mods {
class DateTime {
  public:
    enum class weekday_t : char {
        MONDAY = 0,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY,
        COUNT_,
    };

  public:
    static constexpr time_t timeInMinute{1};
    static constexpr time_t timeInHour{timeInMinute *
                                       DurationUnits::minutesInHour};
    static constexpr time_t timeInDay{timeInHour * DurationUnits::hoursInDay};

  public:
    using date_t = std::chrono::year_month_day;
    using time_t = std::chrono::minutes;

    using year_t = std::chrono::year;
    using month_t = std::chrono::month;
    using day_t = std::chrono::day;

    using hour_t = std::chrono::hours;
    using minute_t = std::chrono::minutes;

  private:
    date_t date_;
    time_t time_;

  public:
    DateTime();

    explicit DateTime(date_t date, time_t time = time_t{0});

    explicit DateTime(year_t year, month_t month, day_t day = day_t{0},
                      hour_t hour = hour_t{0}, minute_t minute = minute_t{0});

  public:
    [[nodiscard]] static auto today() -> DateTime;

    [[nodiscard]] static auto now() -> DateTime;

  public:
    [[nodiscard]] auto getDate() const -> date_t;

    [[nodiscard]] auto getYear() const -> year_t;

    [[nodiscard]] auto getMonth() const -> month_t;

    [[nodiscard]] auto getDay() const -> day_t;

    [[nodiscard]] auto getWeekday() const -> weekday_t;

  public:
    [[nodiscard]] auto getTime() const -> time_t;

    [[nodiscard]] auto getHour() const -> hour_t;

    [[nodiscard]] auto getMinute() const -> minute_t;

  public:
    [[nodiscard]] auto operator<=>(const DateTime &other) const
        -> std::strong_ordering = default;

    [[nodiscard]] auto operator==(const DateTime &other) const
        -> bool = default;

  public:
    [[nodiscard]] auto isToday() const -> bool;

    [[nodiscard]] static auto equalDates(DateTime dt1, DateTime dt2) -> bool;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::optional<DateTime>;

  public:
    [[nodiscard]] auto operator+(const Interval &interval) const -> DateTime;

    auto operator+=(const Interval &interval) -> DateTime &;

    [[nodiscard]] auto operator-(const DateTime &other) const -> DurationUnits;
};
} // namespace hbt::mods
