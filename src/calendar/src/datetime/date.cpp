#include <datetime/date.hpp>
#include <datetime/datetime.hpp>
#include <schema/date.hpp>

namespace clndr::dt {
Date::Date()
    : year_{DateTime::now().getDate().getYear()},
      month_{DateTime::now().getDate().getMonth()},
      day_{DateTime::now().getDate().getDay()} {}

Date::Date(year_t year, month_t month, day_t day)
    : year_{year}, month_{month}, day_{day} {}

[[nodiscard]] auto Date::getYear() const -> year_t { return year_; }

[[nodiscard]] auto Date::getMonth() const -> month_t { return month_; }

[[nodiscard]] auto Date::getDay() const -> day_t { return day_; }

[[nodiscard]] auto Date::ok() const -> bool { return clndr::schema::Date:: }

[[nodiscard]] auto Date::toDuration() const -> duration_t {
    auto ymd{std::chrono::year_month_day(std::chrono::year(year_),
                                         std::chrono::month(month_),
                                         std::chrono::day(day_))};

    return std::chrono::sys_days(ymd).time_since_epoch();
}

[[nodiscard]] auto Date::getWeekday() const -> constants::Weekday {
    auto wd{std::chrono::weekday(std::chrono::sys_days{toDuration()})};

    return static_cast<constants::Weekday>(wd.c_encoding());
}

[[nodiscard]] auto Date::toYMD() const -> std::chrono::year_month_day {
    return {std::chrono::sys_days{toDuration()}};
}

[[nodiscard]] auto Date::operator+(const Interval &interval) const -> Date {
    using std::chrono::sys_days;
    using std::chrono::year_month_day;

    auto addYearsAndMonths(
        [](year_month_day &ymd, const Interval &interval) -> void {
            using namespace std::chrono;

            auto intervalYears{interval[Interval::Unit::YEAR]};
            auto intervalMonths{interval[Interval::Unit::MONTH]};

            switch (interval.getMonthHandling()) {
            case Interval::MonthHandling::WrapAround:
                ymd += years(intervalYears);
                ymd += months(intervalMonths);

                break;

            case Interval::MonthHandling::PreserveRelative:
                auto ym{year_month(ymd.year() / ymd.month())};
                auto oldDay(ymd.day());
                auto daysUntilOldMonthEnd(year_month_day_last(ym / last).day() -
                                          oldDay);

                ym += years(intervalYears);
                ym += months(intervalMonths);
                auto newMonthEnd{year_month_day_last(ym / last).day()};

                // TODO: why is it 27?
                const auto oldDayIsNotContainedByAllMonths{
                    static_cast<size_t>(unsigned(oldDay)) >
                    dt::constants::daysInNonLeapFebruary - 1};

                auto newDay{(oldDayIsNotContainedByAllMonths)
                                ? newMonthEnd - daysUntilOldMonthEnd
                                : oldDay};

                ymd = year_month_day{ym / newDay};
            }
        });

    auto addWeeksAndDays([](year_month_day &ymd, Interval::value_t weeks,
                            Interval::value_t days) -> void {
        auto sysdays{sys_days(ymd)};
        sysdays += std::chrono::days((weeks * Interval::daysInWeek) + days);

        ymd = year_month_day(sysdays);
    });

    auto ymd{toYMD()};

    addYearsAndMonths(ymd, interval);
    addWeeksAndDays(ymd, interval[Interval::Unit::WEEK],
                    interval[Interval::Unit::DAY]);

    return {static_cast<year_t>(static_cast<int>(ymd.year())),
            static_cast<month_t>(static_cast<unsigned>(ymd.month())),
            static_cast<day_t>(static_cast<unsigned>(ymd.day()))};
}

auto Date::operator+=(const Interval &interval) -> Date & {
    *this = *this + interval;

    return *this;
}
} // namespace clndr::dt
