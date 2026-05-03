#include <ymd.hpp>

#include <cassert>

namespace hbt::mods {
[[nodiscard]] constexpr auto YMD::MonthDays::get(month_t month) -> day_t {
    assert(month >= 1 && month <= Duration::monthsInYear);
    return array[month - 1];
}

[[nodiscard]] constexpr auto YMD::MonthDays::get(months_t month) -> day_t {
    return array[static_cast<size_t>(month) - 1];
}

[[nodiscard]] constexpr auto YMD::isLeapYear(year_t year) -> bool {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

[[nodiscard]] constexpr auto YMD::getMonthDays(month_t month, year_t year)
    -> day_t {
    auto daysInMonth{MonthDays::get(month)};

    if (static_cast<months_t>(month) == months_t::FEBRUARY) {
        return (isLeapYear(year)) ? daysInMonth + 1 : daysInMonth;
    }

    return daysInMonth;
}
[[nodiscard]] auto YMD::toChrono() const -> std::chrono::year_month_day {
    using namespace std::chrono;

    auto chrono{year_month_day(year{year_}, month{month_}, day{day_})};

    return chrono;
}

[[nodiscard]] auto YMD::fromChrono(const std::chrono::year_month_day &chrono)
    -> std::expected<YMD, Error> {
    if (!chrono.ok()) {
        return std::unexpected(Error::InvalidChrono);
    }

    return YMD(static_cast<year_t>(int(chrono.year())),
               static_cast<month_t>(unsigned(chrono.month())),
               static_cast<day_t>(unsigned(chrono.day())));
}

[[nodiscard]] auto YMD::ok() const -> bool {
    auto chrono{toChrono()};

    if (!chrono.ok()) {
        throw std::invalid_argument(errorMessage(Error::InvalidArguments));
    }

    return true;
}

YMD::YMD(year_t year, month_t month, day_t day)
    : year_{year}, month_{month}, day_{day} {
    assert(ok());
}

[[nodiscard]] auto YMD::operator<=>(const YMD &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto YMD::operator==(const YMD &other) const -> bool = default;

auto YMD::today() -> YMD {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};
    auto chrono{year_month_day(today)};

    assert(chrono.ok());

    auto ymdFromChrono{fromChrono(chrono)};
    if (!ymdFromChrono) {
        throw std::invalid_argument(errorMessage(ymdFromChrono.error()));
    }

    return ymdFromChrono.value();
}

[[nodiscard]] auto YMD::getYear() const -> year_t { return year_; }

[[nodiscard]] auto YMD::getMonth() const -> month_t { return month_; }

[[nodiscard]] auto YMD::getDay() const -> day_t { return day_; }

[[nodiscard]] auto YMD::getWeekday() const -> weekday_t {
    auto chrono{toChrono()};

    return static_cast<weekday_t>(std::chrono::weekday{chrono}.c_encoding());
}

auto YMD::addYears(value_t years) -> void { year_ += years; }

auto YMD::addMonths(value_t months) -> void {
    addYears(months / Duration::monthsInYear);

    months %= Duration::monthsInYear;

    if (month_ + months > Duration::monthsInYear) {
        addYears(1);

        month_ = static_cast<month_t>(months_t::DECEMBER) + months;
    }
}

auto YMD::addWeeks(value_t weeks) -> void {
    addDays(weeks * Duration::daysInWeek);
}

auto YMD::addDays(value_t days) -> void {
    for (auto monthDays{MonthDays::get(getMonth())}; days > monthDays;
         days -= monthDays) {
        addMonths(1);
    }

    if (auto currentMonthDays{MonthDays::get(getMonth())};
        days + day_ > currentMonthDays) {
        addMonths(1);
        days -= currentMonthDays;
    }

    day_ += days;
}

auto YMD::addDuration(const Duration &duraion) -> void {}
}; // namespace hbt::mods
