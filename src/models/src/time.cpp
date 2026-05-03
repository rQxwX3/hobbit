#include <time.hpp>

namespace hbt::mods {
[[nodiscard]] auto Time::valueValidator(value_t value) -> value_t {
    if (value < value_t{0}) {
        throw std::invalid_argument(errorMessage(Error::InvalidValue));
    }

    return value % timeInDay;
}

[[nodiscard]] auto Time::hourValidator(hours_t hour) -> hours_t {
    auto count{static_cast<int64_t>(hour.count())};

    if (count < minHourValue || maxHourValue < count) {
        throw std::invalid_argument(errorMessage(Error::InvalidHour));
    }

    return hour;
}

[[nodiscard]] auto Time::minuteValidator(minutes_t minute) -> minutes_t {
    auto count{static_cast<int64_t>(minute.count())};

    if (count < minMinuteValue || maxMinuteValue < count) {
        throw std::invalid_argument(errorMessage(Error::InvalidMinute));
    }

    return minute;
}

[[nodiscard]] auto Time::valueHourValidator(int8_t hour) -> int8_t {
    if (hour < minHourValue || maxHourValue < hour) {
        throw std::invalid_argument(errorMessage(Error::InvalidHour));
    }

    return hour;
}

[[nodiscard]] auto Time::valueMinuteValidator(int8_t minute) -> int8_t {
    if (minute < minMinuteValue || maxMinuteValue < minute) {
        throw std::invalid_argument(errorMessage(Error::InvalidMinute));
    }

    return minute;
}

Time::Time(value_t value) : value_{valueValidator(value)} {}

Time::Time(hours_t hours, minutes_t minutes)
    : value_{duration_cast<minutes_t>(hourValidator(hours)) +
             minuteValidator(minutes)} {}

Time::Time(int8_t hours, int8_t minutes)
    : value_{valueMinuteValidator(minutes) +
             (valueHourValidator(hours) * Interval::minutesInHour)} {}

[[nodiscard]] auto Time::now() -> Time {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return duration_cast<value_t>(now - today);
}

[[nodiscard]] auto Time::getValue() const -> value_t { return value_; }

[[nodiscard]] auto Time::getHour() const -> hours_t {
    auto hms{std::chrono::hh_mm_ss<value_t>(value_)};

    return hms.hours();
}

[[nodiscard]] auto Time::getMinute() const -> minutes_t {
    auto hms{std::chrono::hh_mm_ss<value_t>(value_)};

    return hms.minutes();
}

[[nodiscard]] auto Time::operator<=>(const Time &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto Time::operator==(const Time &other) const -> bool = default;

[[nodiscard]] auto Time::operator!=(const Time &other) const -> bool = default;

[[nodiscard]] auto Time::operator+(const Interval &interval) const
    -> std::pair<Time, bool> {
    auto result{
        value_ +
        value_t{interval.getUnitValue(Interval::unit_t::HOUR) * timeInHour} +
        value_t{interval.getUnitValue(Interval::unit_t::MINUTE)}};

    return {Time(result % timeInDay), result / timeInDay != 0};
}

[[nodiscard]] auto Time::minutesBetween(const Time &t1, const Time &t2)
    -> Interval {
    auto diff{(t1 > t2) ? t1.getValue() - t2.getValue()
                        : t2.getValue() - t1.getValue()};

    return Interval::minutes(diff.count());
}
} // namespace hbt::mods
