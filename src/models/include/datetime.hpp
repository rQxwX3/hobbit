#pragma once

#include <date.hpp>
#include <interval.hpp>
#include <time.hpp>

#include <nlohmann/json.hpp>

#include <optional>

namespace hbt::mods {
class DateTime {
  public:
    using year_t = Date::year_t;
    using month_t = Date::month_t;
    using day_t = Date::day_t;
    using weekday_t = Date::weekday_t;

    using time_value_t = Time::value_t;
    using hours_t = Time::hours_t;
    using minutes_t = Time::minutes_t;

  private:
    mods::Date date_;
    mods::Time time_;

  public:
    DateTime();

    explicit DateTime(mods::Date date, mods::Time time);

    explicit DateTime(mods::Date date,
                      time_value_t timeValue = time_value_t{0});

    explicit DateTime(year_t year, month_t month, day_t day,
                      hours_t hours = hours_t{0},
                      minutes_t minutes = minutes_t{0});

  public:
    [[nodiscard]] static auto now() -> DateTime;

  public:
    [[nodiscard]] auto getDate() const -> mods::Date;

    [[nodiscard]] auto getTime() const -> mods::Time;

  public:
    [[nodiscard]] static auto equalDate(DateTime dt1, DateTime dt2) -> bool;

    [[nodiscard]] static auto equalTime(DateTime dt1, DateTime dt2) -> bool;

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

  public:
    [[nodiscard]] static auto getDiff(const DateTime &dt1, const DateTime &dt2)
        -> Duration;
};
} // namespace hbt::mods
