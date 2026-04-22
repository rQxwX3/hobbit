#include <time.hpp>

namespace hbt::mods {
[[nodiscard]] auto Time::valueValidator(value_t value) -> value_t {
    if (value < value_t{0}) {
        throw std::invalid_argument(
            "Time: can't create Time object with negative value");
    }

    return value % timeInDay;
}

Time::Time(value_t value) : value_{valueValidator(value)} {}

Time::Time(hours_t hours, minutes_t minutes)
    : value_{valueValidator(duration_cast<minutes_t>(hours) + minutes)} {}

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

[[nodiscard]] auto Time::operator-(const Time &other) const -> DurationUnits {
    return DurationUnits::fromUnit(DurationUnits::unit_t::MINUTE,
                                   value_.count() - other.getValue().count());
}
} // namespace hbt::mods
