#include <date.hpp>

#include <cassert>
#include <chrono>

namespace hbt::mods {
[[nodiscard]] auto Date::valueYMDValidator(value_t year, value_t month,
                                           value_t day) -> ymd_t {
    if (month < 1 || Interval::monthsInYear < month) {
        throw std::invalid_argument(errorMessage(Error::InvalidMonth));
    }

    if (day < 1 || getDaysInMonth(month, year) < day) {
        throw std::invalid_argument(errorMessage(Error::InvalidDay));
    }

    return ymd_t{year_t(year), month_t(month), day_t(day)};
}

Date::Date() : ymd_{Date::today().getYMD()} {}

Date::Date(ymd_t ymd) : ymd_{ymd} {}

Date::Date(year_t year, month_t month, day_t day)
    : ymd_{ymd_t(year, month, day)} {}

Date::Date(value_t year, value_t month, value_t day)
    : ymd_{valueYMDValidator(year, month, day)} {}

[[nodiscard]] auto Date::getYMD() const -> ymd_t { return ymd_; }

[[nodiscard]] auto Date::getWeekday() const -> weekday_t {
    auto wd{std::chrono::weekday(std::chrono::sys_days{ymd_})};

    return static_cast<weekday_t>(wd.c_encoding());
}

[[nodiscard]] auto Date::getYear() const -> year_t { return ymd_.year(); }

[[nodiscard]] auto Date::getMonth() const -> month_t { return ymd_.month(); }

[[nodiscard]] auto Date::getDay() const -> day_t { return ymd_.day(); }

[[nodiscard]] auto Date::today() -> Date {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return ymd_t(today);
}

[[nodiscard]] auto Date::isToday() const -> bool { return *this == today(); }

[[nodiscard]] auto Date::ok() const -> bool { return ymd_.ok(); }

[[nodiscard]] auto Date::operator<=>(const Date &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto Date::operator==(const Date &other) const -> bool = default;

[[nodiscard]] auto Date::operator!=(const Date &other) const -> bool = default;

[[nodiscard]] auto Date::operator+(const Interval &interval) const -> Date {
    using namespace std::chrono;

    auto resYMD{getYMD()};

    resYMD += years{interval.getUnitValue(Interval::unit_t::YEAR)};
    resYMD += months{interval.getUnitValue(Interval::unit_t::MONTH)};

    auto resSD{sys_days(resYMD)};

    resSD += days{interval.getUnitValue(Interval::unit_t::WEEK) *
                  Interval::daysInWeek};
    resSD += days{interval.getUnitValue(Interval::unit_t::DAY)};

    auto res{Date(ymd_t{resSD})};
    assert(res.ok());

    return res;
}

[[nodiscard]] auto Date::daysBetween(const Date &first, const Date &second)
    -> Interval {
    auto earlier{std::min(first, second)};
    auto later{std::max(first, second)};
    assert(later >= earlier);

    auto earlierSysdays{std::chrono::sys_days(earlier.ymd_)};
    auto laterSysdays{std::chrono::sys_days(later.ymd_)};

    return Interval::days((laterSysdays - earlierSysdays).count());
}

auto Date::operator+=(const Interval &interval) -> Date & {
    *this = *this + interval;

    return *this;
}
} // namespace hbt::mods
