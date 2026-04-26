#pragma once

#include <duration.hpp>
#include <interval.hpp>

#include <chrono>

namespace hbt::mods {
class Time {
  public:
    using hours_t = std::chrono::hours;
    using minutes_t = std::chrono::minutes;

    using value_t = minutes_t;

    enum class Error : uint8_t {
        InvalidValue,
        InvalidHour,
        InvalidMinute,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidValue:
            return "Time: can't create Time object with negative value";

        case Error::InvalidHour:
            return "Time: provided hour value is invalid";

        case Error::InvalidMinute:
            return "Time: provided minute value is invalid";
        }
    }

  public:
    static constexpr value_t timeInMinute{1};
    static constexpr value_t timeInHour{timeInMinute * Duration::minutesInHour};
    static constexpr value_t timeInDay{timeInHour * Duration::hoursInDay};

  private:
    value_t value_;

  private:
    [[nodiscard]] static auto valueValidator(value_t value) -> value_t;

    [[nodiscard]] static auto hourValidator(hours_t hour) -> hours_t;

    [[nodiscard]] static auto minuteValidator(minutes_t minute) -> minutes_t;

  public:
    Time(value_t value);

    Time(hours_t hours, minutes_t minutes);

  public:
    [[nodiscard]] static auto now() -> Time;

  public:
    [[nodiscard]] auto getValue() const -> value_t;

    [[nodiscard]] auto getHour() const -> hours_t;

    [[nodiscard]] auto getMinute() const -> minutes_t;

  public:
    [[nodiscard]] auto operator<=>(const Time &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const Time &other) const -> bool;

    [[nodiscard]] auto operator+(const Interval &interval) const
        -> std::pair<Time, bool>;

  public:
    [[nodiscard]] static auto getDiff(const Time &t1, const Time &t2)
        -> Duration;
};
} // namespace hbt::mods
