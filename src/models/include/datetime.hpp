#pragma once

#include <interval.hpp>
#include <weekdays.hpp>

#include <nlohmann/json.hpp>

#include <chrono>
#include <expected>

namespace hbt::mods {
class DateTime {
  public:
    using duration_t = std::chrono::minutes;
    using value_t = std::chrono::sys_time<duration_t>;

    using weekday_t = Weekdays::Week;

  public:
    struct Date {
      public:
        using year_t = int16_t;
        using month_t = uint8_t;
        using day_t = uint8_t;

        using duration_t = std::chrono::days;

      public:
        /* order must not be changed */
        year_t year;
        month_t month;
        day_t day;

      public:
        [[nodiscard]] auto ok() const -> bool {
            auto ymd{std::chrono::year_month_day(std::chrono::year(year),
                                                 std::chrono::month(month),
                                                 std::chrono::day(day))};

            return ymd.ok();
        }

        [[nodiscard]] auto toDuration() const -> duration_t {
            auto ymd{std::chrono::year_month_day(std::chrono::year(year),
                                                 std::chrono::month(month),
                                                 std::chrono::day(day))};

            return std::chrono::sys_days(ymd).time_since_epoch();
        }

        [[nodiscard]] auto operator<=>(const Date &other) const
            -> std::strong_ordering = default;
    };

    struct Time {
      public:
        using hour_t = uint8_t;
        using minute_t = uint8_t;

        using duration_t = std::chrono::minutes;

      private:
        static constexpr hour_t minHourValue{0};
        static constexpr hour_t maxHourValue{23};

        static constexpr minute_t minMinuteValue{0};
        static constexpr minute_t maxMinuteValue{59};

      public:
        /* order must not be changed */
        hour_t hour;
        minute_t minute;

      public:
        [[nodiscard]] auto ok() const -> bool {
            return (minHourValue <= hour && hour <= maxHourValue) &&
                   (minMinuteValue <= minute && minute <= maxMinuteValue);
        }

        [[nodiscard]] auto toDuration() const -> duration_t {
            return duration_t((hour * Interval::minutesInHour) + minute);
        }

        [[nodiscard]] auto operator<=>(const Time &other) const
            -> std::strong_ordering = default;
    };

  private:
  public:
    enum class Error : uint8_t {
        InvalidDate,
        InvalidTime,

        ISO8601RegexMismatch,
        ISO8601UnitNotMatched,

        ISO8601InvalidDate,
        ISO8601InvalidTime,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidDate:
            return "DateTime provided date is invalid";

        case Error::InvalidTime:
            return "DateTime provided time is invalid";

        case Error::ISO8601RegexMismatch:
            return "DateTime: provided input didn't match regex";

        case Error::ISO8601UnitNotMatched:
            return "DateTime: provided input doesn't contain "
                   "required unit(s)";

        case Error::ISO8601InvalidDate:
            return "DateTime: provided input contains invalid date";

        case Error::ISO8601InvalidTime:
            return "DateTime: provided input contains invalid time";

        default:
            std::unreachable();
        }
    }

  private:
    value_t value_;

  private:
    [[nodiscard]] auto getDaysSinceEpoch() const -> Date::duration_t;

    [[nodiscard]] auto getMinutesSinceMidnight() const -> Time::duration_t;

  public:
    DateTime();

    DateTime(value_t value);

    DateTime(Date date, Time time = {.hour = 0, .minute = 0});

  public:
    [[nodiscard]] static auto now() -> DateTime;

  public:
    [[nodiscard]] auto getDate() const -> Date;

    [[nodiscard]] auto getTime() const -> Time;

    [[nodiscard]] auto getWeekday() const -> weekday_t;

  public:
    [[nodiscard]] static auto equalDate(DateTime dt1, DateTime dt2) -> bool;

    [[nodiscard]] static auto equalTime(DateTime dt1, DateTime dt2) -> bool;

  public:
    [[nodiscard]] auto toISO8601String() const -> std::string;

    [[nodiscard]] static auto fromISO8601String(const std::string &string)
        -> std::expected<DateTime, Error>;

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
} // namespace hbt::mods
