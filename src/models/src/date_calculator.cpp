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

[[nodiscard]] auto DateCalculator::daysBetween(const Date &first,
                                               const Date &second)
    -> day_diff_t {
    const auto &later{(first > second) ? first : second};
    const auto &earlier{(first > second) ? second : first};

    auto laterSysDays{std::chrono::sys_days(later.getChronoYMD())};
    auto earlierSysDays{std::chrono::sys_days(earlier.getChronoYMD())};

    return (laterSysDays - earlierSysDays).count();
}

// [[nodiscard]] auto DateCalculator::getSignedDateDiff(const Date &later,
//                                                      const Date &earlier)
//     -> SignedDateDiff {
//     assert(later >= earlier);
//
//     return SignedDateDiff{.years = later.getYear() - earlier.getYear(),
//                           .months = later.getMonth() - earlier.getMonth(),
//                           .days = later.getDay() - earlier.getDay()};
// }
//
// [[nodiscard]] auto
// DateCalculator::getBorrowMonthYear(Date::month_t currentMonth,
//                                    Date::year_t currentYear)
//     -> std::pair<Date::month_t, Date::year_t> {
//     auto borrowMonth{currentMonth};
//     auto borrowYear{currentYear};
//
//     if (borrowMonth == static_cast<YMD::month_t>(months_t::JANUARY)) {
//         return {static_cast<YMD::month_t>(months_t::DECEMBER), --borrowYear};
//     }
//
//     return {--borrowMonth, borrowYear};
// }
//
// [[nodiscard]] auto DateCalculator::differenceCalendar(const Date &first,
//                                                       const Date &second)
//     -> Duration {
//     const auto &later{(first > second) ? first : second};
//     const auto &earlier{(first > second) ? second : first};
// }
} // namespace hbt::mods::util
