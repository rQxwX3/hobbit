#include <datetime.hpp>

#include <chrono>

namespace hbt::mods {
using std::chrono::floor, std::chrono::system_clock, std::chrono::duration_cast;

[[nodiscard]] auto Date::ymdValidator(ymd_t ymd) -> ymd_t {
    if (!ymd.ok()) {
        throw std::invalid_argument(
            "Date: provided year-month-day value is not valid");
    }

    return ymd;
}

Date::Date(ymd_t ymd) : ymd_{ymdValidator(ymd)} {}

Date::Date(year_t year, month_t month, day_t day)
    : ymd_{ymdValidator({year, month, day})} {}

[[nodiscard]] auto Date::getYMD() const -> ymd_t { return ymd_; }

[[nodiscard]] auto Date::getWeekday() const -> weekday_t {
    return static_cast<weekday_t>(std::chrono::weekday{ymd_}.c_encoding());
}

[[nodiscard]] auto Date::getYear() const -> year_t { return ymd_.year(); }

[[nodiscard]] auto Date::getMonth() const -> month_t { return ymd_.month(); }

[[nodiscard]] auto Date::getDay() const -> day_t { return ymd_.day(); }

[[nodiscard]] auto Date::today() -> Date {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return {ymd_t(today)};
}

[[nodiscard]] auto Date::isToday() const -> bool {
    return ymd_ == today().getYMD();
}

[[nodiscard]] auto Date::operator==(const Date &other) const -> bool = default;

[[nodiscard]] auto Date::operator<=>(const Date &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto Date::operator+(const Interval &interval) const -> Date {
    auto clampToMonthEnd{[](ymd_t ymd) -> auto {
        return std::chrono::year_month_day_last{
            ymd.year(), std::chrono::month_day_last{ymd.month()}};
    }};

    auto wrapAroundMonth{[](ymd_t ymd) -> auto {
        auto lastDay{std::chrono::year_month_day_last{
            ymd.year(), std::chrono::month_day_last{ymd.month()}}};
        auto overflow{static_cast<size_t>(unsigned{ymd.day()}) -
                      static_cast<size_t>(unsigned{lastDay.day()})};

        return std::chrono::year_month_day{std::chrono::sys_days{lastDay} +
                                           std::chrono::days{overflow}};
    }};

    auto newYMD{
        ymd_ +
        std::chrono::years(interval.getUnitValue(Interval::unit_t::YEAR)) +
        std::chrono::months(interval.getUnitValue(Interval::unit_t::MONTH))};

    if (!newYMD.ok()) {
        switch (interval.getMonthHandling()) {
        case Interval::MonthHandling::CUT_OFF:
            newYMD = clampToMonthEnd(newYMD);
            break;

        case Interval::MonthHandling::WRAP_AROUND:
            newYMD = wrapAroundMonth(newYMD);
            break;
        }
    }

    auto sysdays{
        std::chrono::sys_days{newYMD} +
        std::chrono::days(interval.getUnitValue(Interval::unit_t::WEEK) *
                          DurationUnits::daysInWeek) +
        std::chrono::days(interval.getUnitValue(Interval::unit_t::DAY))};

    return {ymd_t(sysdays)};
}

[[nodiscard]] auto Date::operator-(const Interval &interval) const -> Date {
    auto newYMD{
        ymd_ -
        std::chrono::years(interval.getUnitValue(Interval::unit_t::YEAR)) -
        std::chrono::months(interval.getUnitValue(Interval::unit_t::MONTH))};

    return {newYMD};
}

auto Date::operator+=(const Interval &interval) -> Date & {
    *this = *this + interval;

    return *this;
}

[[nodiscard]] auto Date::operator-(const Date &other) const -> DurationUnits {
    auto thisTP{std::chrono::sys_days{ymd_}};
    auto otherTP{std::chrono::sys_days{other.getYMD()}};

    return DurationUnits::fromUnit(DurationUnits::unit_t::DAY,
                                   (thisTP - otherTP).count());
}
} // namespace hbt::mods
