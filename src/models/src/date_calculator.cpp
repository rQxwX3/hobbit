#include <date_calculator.hpp>

namespace hbt::mods::util {

[[nodiscard]] constexpr auto DateCalculator::MonthDays::get(Date::month_t month)
    -> Date::day_t {
    assert(month >= 1 && month <= Duration::monthsInYear);
    return array[month - 1];
}

[[nodiscard]] constexpr auto DateCalculator::MonthDays::get(months_t month)
    -> Date::day_t {
    return array[static_cast<size_t>(month) - 1];
}

[[nodiscard]] constexpr auto DateCalculator::isLeapYear(Date::year_t year)
    -> bool {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

[[nodiscard]] constexpr auto DateCalculator::getMonthDays(Date::month_t month,
                                                          Date::year_t year)
    -> Date::day_t {
    auto daysInMonth{MonthDays::get(month)};

    if (static_cast<months_t>(month) == months_t::FEBRUARY) {
        return (isLeapYear(year)) ? daysInMonth + 1 : daysInMonth;
    }

    return daysInMonth;
}

[[nodiscard]] auto DateCalculator::getDateDifference(const Date &later,
                                                     const Date &earlier)
    -> DateDifference {
    assert(later > earlier);

    return DateDifference{.years = later.getYear() - earlier.getYear(),
                          .months = later.getMonth() - earlier.getMonth(),
                          .days = later.getDay() - earlier.getDay()};
}

[[nodiscard]] auto DateCalculator::getBorrowMonthYear(const Date &later)
    -> std::pair<Date::month_t, Date::year_t> {
    auto borrowMonth{later.getMonth()};
    auto borrowYear{later.getYear()};

    if (borrowMonth == static_cast<YMD::month_t>(months_t::JANUARY)) {
        return {static_cast<YMD::month_t>(months_t::DECEMBER), --borrowYear};
    }

    return {--borrowMonth, borrowYear};
}

[[nodiscard]] auto DateCalculator::difference(const Date &d1, const Date &d2)
    -> Duration {
    const auto &earlier{(d1 < d2) ? d1 : d2};
    const auto &later{(d1 < d2) ? d2 : d1};

    auto dateDiff{getDateDifference(later, earlier)};

    if (dateDiff.days < 0) {
        dateDiff.months -= 1;

        auto borrowMonthYear{getBorrowMonthYear(later)};

        dateDiff.days +=
            getMonthDays(borrowMonthYear.first, borrowMonthYear.second);
    }

    if (dateDiff.months < 0) {
        dateDiff.years -= 1;
        dateDiff.months += Duration::monthsInYear;
    }

    assert(dateDiff.years >= 0 && dateDiff.months >= 0 && dateDiff.days >= 0);

    return Duration({.years = static_cast<Duration::value_t>(dateDiff.years),
                     .months = static_cast<Duration::value_t>(dateDiff.months),
                     .days = static_cast<Duration::value_t>(dateDiff.days)});
}
} // namespace hbt::mods::util
