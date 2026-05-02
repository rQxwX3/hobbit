#include <datetime.hpp>

#include <chrono>

namespace hbt::mods {
using std::chrono::floor, std::chrono::system_clock, std::chrono::duration_cast;

[[nodiscard]] auto Date::chronoYMDValidator(chrono_ymd_t chronoYMD)
    -> chrono_ymd_t {
    if (!chronoYMD.ok()) {
        throw std::invalid_argument(errorMessage(Error::InvalidChronoYMD));
    }

    return chronoYMD;
}

[[nodiscard]] auto Date::ymdValidator(YMD ymd) -> chrono_ymd_t {
    auto chronoYMD{std::chrono::year_month_day{std::chrono::year(ymd.year),
                                               std::chrono::month(ymd.month),
                                               std::chrono::day(ymd.day)}};

    if (!chronoYMD.ok()) {
        throw std::invalid_argument(errorMessage(Error::InvalidYMD));
    }

    return chronoYMD;
}

Date::Date(chrono_ymd_t chronoYMD)
    : chronoYMD_{chronoYMDValidator(chronoYMD)} {}

Date::Date(YMD ymd) : chronoYMD_{ymdValidator(ymd)} {}

Date::Date(year_t year, month_t month, day_t day)
    : chronoYMD_{chronoYMDValidator({year, month, day})} {}

[[nodiscard]] auto Date::getChronoYMD() const -> chrono_ymd_t {
    return chronoYMD_;
}

[[nodiscard]] auto Date::getWeekday() const -> weekday_t {
    return static_cast<weekday_t>(
        std::chrono::weekday{chronoYMD_}.c_encoding());
}

[[nodiscard]] auto Date::getYear() const -> year_t { return chronoYMD_.year(); }

[[nodiscard]] auto Date::getMonth() const -> month_t {
    return chronoYMD_.month();
}

[[nodiscard]] auto Date::getDay() const -> day_t { return chronoYMD_.day(); }

[[nodiscard]] auto Date::today() -> Date {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return {chrono_ymd_t(today)};
}

[[nodiscard]] auto Date::isToday() const -> bool {
    return chronoYMD_ == today().getChronoYMD();
}

[[nodiscard]] auto Date::operator==(const Date &other) const -> bool = default;

[[nodiscard]] auto Date::operator!=(const Date &other) const -> bool = default;

[[nodiscard]] auto Date::operator<=>(const Date &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto Date::operator+(const Interval &interval) const -> Date {
    auto units{interval.getDuration().getUnits()};

    auto newChronoYMD{chronoYMD_ + std::chrono::years(units.years) +
                      std::chrono::months(units.months)};

    switch (interval.getMonthHandling()) {
    case Interval::MonthHandling::CLAMP_TO_END:
        newChronoYMD = clampToMonthEnd(newChronoYMD);
        break;

    case Interval::MonthHandling::RESOLVE_OVERFLOW:
        newChronoYMD = resolveMonthOverflow(newChronoYMD);
        break;
    }

    auto sysdays{std::chrono::sys_days{newChronoYMD} +
                 std::chrono::days(units.weeks * Duration::daysInWeek) +
                 std::chrono::days(units.days)};

    return {chrono_ymd_t(sysdays)};
}

[[nodiscard]] auto Date::operator-(const Interval &interval) const -> Date {
    auto units{interval.getDuration().getUnits()};

    auto newChronoYMD{chronoYMD_ - std::chrono::years(units.years) -
                      std::chrono::months(units.months)};

    switch (interval.getMonthHandling()) {
    case Interval::MonthHandling::CLAMP_TO_END:
        newChronoYMD = clampToMonthEnd(newChronoYMD);
        break;

    case Interval::MonthHandling::RESOLVE_OVERFLOW:
        newChronoYMD = resolveMonthOverflow(newChronoYMD);
        break;
    }

    auto sysdays{std::chrono::sys_days(newChronoYMD) -
                 std::chrono::days(units.weeks * Duration::daysInWeek) -
                 std::chrono::days(units.days)};

    return {chrono_ymd_t(sysdays)};
}

auto Date::operator+=(const Interval &interval) -> Date & {
    *this = *this + interval;

    return *this;
}

[[nodiscard]] auto Date::getDiff(const Date &d1, const Date &d2) -> Duration {
    auto getDateDiff([](const Date &d1, const Date &d2) -> Duration {
        auto d1ChronoYMD{d1.getChronoYMD()};
        auto d2ChronoYMD{d2.getChronoYMD()};

        return Duration(Duration::Units{
            .years = static_cast<Duration::value_t>(
                (d1ChronoYMD.year() - d2ChronoYMD.year()).count()),
            .months = static_cast<Duration::value_t>(
                (d1ChronoYMD.month() - d2ChronoYMD.month()).count()),
            .days = static_cast<Duration::value_t>(
                (d1ChronoYMD.day() - d2ChronoYMD.day()).count()),
        });
    });

    return (d1 > d2) ? getDateDiff(d1, d2) : getDateDiff(d2, d1);
}
} // namespace hbt::mods
