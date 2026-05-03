#include <date_calculator.hpp>
#include <datetime.hpp>

#include <chrono>

namespace hbt::mods {
using std::chrono::floor, std::chrono::system_clock, std::chrono::duration_cast;

[[nodiscard]] auto Date::ymdValidator(YMD ymd) -> ymd_t {
    if (!ymd.ok()) {
        throw std::invalid_argument(errorMessage(Error::InvalidYMD));
    }

    return ymd;
}

Date::Date() : YMD_{Date::today().getYMD()} {}

Date::Date(ymd_t ymd) : YMD_{ymdValidator(ymd)} {}

Date::Date(year_t year, month_t month, day_t day)
    : YMD_{ymdValidator({year, month, day})} {}

[[nodiscard]] auto Date::getYMD() const -> ymd_t { return YMD_; }

[[nodiscard]] auto Date::getChronoYMD() const -> std::chrono::year_month_day {
    return YMD_.toChrono();
}

[[nodiscard]] auto Date::getWeekday() const -> weekday_t {
    return YMD_.getWeekday();
}

[[nodiscard]] auto Date::getYear() const -> year_t { return YMD_.getYear(); }

[[nodiscard]] auto Date::getMonth() const -> month_t { return YMD_.getMonth(); }

[[nodiscard]] auto Date::getDay() const -> day_t { return YMD_.getDay(); }

[[nodiscard]] auto Date::today() -> Date { return {YMD::today()}; }

[[nodiscard]] auto Date::isToday() const -> bool {
    return YMD_ == today().getYMD();
}

[[nodiscard]] auto Date::operator<=>(const Date &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto Date::operator==(const Date &other) const -> bool = default;

[[nodiscard]] auto Date::operator!=(const Date &other) const -> bool = default;

[[nodiscard]] auto Date::operator+(const Interval &interval) const -> Date {
    using namespace std::chrono;

    auto units{interval.getDuration().getUnits()};

    auto base{sys_days{YMD_.toChrono()}};

    // adding all concrete calendar-agnostic units
    auto shifted{base + days(units.weeks * Duration::daysInWeek) +
                 days(units.days)};

    auto shiftedChronoYMD{std::chrono::year_month_day{shifted}};
    assert(shiftedChronoYMD.ok());

    auto newChronoYMD{shiftedChronoYMD + months(units.months) +
                      years(units.years)};

    switch (interval.getMonthHandling()) {
    case Interval::MonthHandling::CLAMP_TO_END:
        // newChronoYMD = clampToMonthEnd(newChronoYMD);
        break;

    case Interval::MonthHandling::RESOLVE_OVERFLOW:
        // newChronoYMD = resolveMonthOverflow(newChronoYMD);
        break;
    }

    // return {newChronoYMD};
    return {};
}

// [[nodiscard]] auto Date::operator+(const Interval &interval) const -> Date {
//     using namespace std::chrono;
//
//     auto units{interval.getDuration().getUnits()};
//     auto resYMD{getYMD()};
//
//     resYMD.year += interval.getUnitValue(Interval::unit_t::YEAR);
//     resYMD.day +=
//         interval.getUnitValue(Interval::unit_t::WEEK) * Duration::daysInWeek
//         + interval.getUnitValue(Interval::unit_t::DAY);
//
//     auto newChronoYMD{shiftedChronoYMD + months(units.months) +
//                       years(units.years)};
//
//     switch (interval.getMonthHandling()) {
//     case Interval::MonthHandling::CLAMP_TO_END:
//         newChronoYMD = clampToMonthEnd(newChronoYMD);
//         break;
//
//     case Interval::MonthHandling::RESOLVE_OVERFLOW:
//         newChronoYMD = resolveMonthOverflow(newChronoYMD);
//         break;
//     }
//
//     return {newChronoYMD};
// }

[[nodiscard]] auto Date::operator-(const Interval &interval) const -> Date {
    using namespace std::chrono;

    auto units{interval.getDuration().getUnits()};

    // subtracting all units except months
    auto sysdays{sys_days(YMD_.toChrono() - years(units.years)) -
                 days(units.weeks * Duration::daysInWeek) - days(units.days)};

    auto newChronoYMD{std::chrono::year_month_day(sysdays) -
                      months(units.months)};

    switch (interval.getMonthHandling()) {
    case Interval::MonthHandling::CLAMP_TO_END:
        // newChronoYMD = clampToMonthEnd(newChronoYMD);
        break;

    case Interval::MonthHandling::RESOLVE_OVERFLOW:
        // newChronoYMD = resolveMonthOverflow(newChronoYMD);
        break;
    }

    return {};
    // return {newChronoYMD};
}

auto Date::operator+=(const Interval &interval) -> Date & {
    *this = *this + interval;

    return *this;
}

auto Date::operator-=(const Interval &interval) -> Date & {
    *this = *this - interval;

    return *this;
}

[[nodiscard]] auto Date::diff(const Date &first, const Date &second)
    -> Duration {
    return util::DateCalculator::difference(first, second);
}
} // namespace hbt::mods
