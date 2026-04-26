#include <time.hpp>

namespace hbt::mods {
[[nodiscard]] auto Time::valueValidator(value_t value) -> value_t {
    if (value < value_t{0}) {
        throw std::invalid_argument(errorMessage(Error::InvalidValue));
    }

    return value % timeInDay;
}

[[nodiscard]] auto Time::hourValidator(hours_t hour) -> hours_t {
    if (auto count{hour.count()}; count < 0 || count >= Duration::hoursInDay) {
        throw std::invalid_argument(errorMessage(Error::InvalidHour));
    }

    return hour;
}

[[nodiscard]] auto Time::minuteValidator(minutes_t minute) -> minutes_t {
    if (auto count{minute.count()};
        count < 0 || count >= Duration::minutesInHour) {
        throw std::invalid_argument(errorMessage(Error::InvalidMinute));
    }

    return minute;
}

Time::Time(value_t value) : value_{valueValidator(value)} {}

Time::Time(hours_t hours, minutes_t minutes)
    : value_{duration_cast<minutes_t>(hourValidator(hours)) +
             minuteValidator(minutes)} {}

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

[[nodiscard]] auto Time::operator+(const Interval &interval) const
    -> std::pair<Time, bool> {
    auto result{
        value_ +
        value_t{interval.getUnitValue(Interval::unit_t::HOUR) * timeInHour} +
        value_t{interval.getUnitValue(Interval::unit_t::MINUTE)}};

    return {Time(result % timeInDay), result / timeInDay != 0};
}

[[nodiscard]] auto Time::getDiff(const Time &t1, const Time &t2) -> Duration {
    auto diff{(t1 > t2) ? t1.getValue() - t2.getValue()
                        : t2.getValue() - t1.getValue()};

    return Duration::fromUnit(Duration::unit_t::MINUTE, diff.count());
}
} // namespace hbt::mods
