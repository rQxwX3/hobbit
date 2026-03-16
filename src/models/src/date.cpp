#include <date.hpp>

#include <chrono>
#include <format>

namespace hbt::mods {
Date::Date()
    : ymd_{floor<std::chrono::days>(std::chrono::system_clock::now())} {}

Date::Date(std::chrono::year_month_day ymd) : ymd_{ymd} {}

Date::Date(std::chrono::year year, std::chrono::month month,
           std::chrono::day day)
    : ymd_{std::chrono::year_month_day{std::chrono::year{year},
                                       std::chrono::month{month},
                                       std::chrono::day{day}}} {}

[[nodiscard]] auto Date::getYMD() const -> std::chrono::year_month_day {
    return ymd_;
}

[[nodiscard]] auto Date::today() -> Date {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return Date(year_month_day(today));
}

[[nodiscard]] auto Date::getYear() const -> std::chrono::year {
    return ymd_.year();
}

[[nodiscard]] auto Date::getMonth() const -> std::chrono::month {
    return ymd_.month();
}

[[nodiscard]] auto Date::getDay() const -> std::chrono::day {
    return ymd_.day();
}

[[nodiscard]] auto Date::getWeekday() const -> std::chrono::weekday {
    return std::chrono::weekday{ymd_};
}

[[nodiscard]] auto Date::isToday() const -> bool {
    return Date::today().getYMD() == ymd_;
}

[[nodiscard]] auto Date::toYMDString() const -> std::string {
    return std::format("{:%Y-%m-%d}", ymd_);
}

[[nodiscard]] auto Date::fromYMDString(const std::string &ymdString) -> Date {
    constexpr int yearDigits{4};
    constexpr int monthDigits{2};
    constexpr int dayDigits{2};

    auto yearString{std::stoi(ymdString.substr(0, yearDigits))};

    auto monthString{static_cast<unsigned>(
        std::stoi(ymdString.substr(yearDigits + 1, monthDigits)))};

    auto dayString{static_cast<unsigned>(std::stoi(
        ymdString.substr(yearDigits + 1 + monthDigits + 1, dayDigits)))};

    std::chrono::year_month_day ymd{std::chrono::year{yearString},
                                    std::chrono::month{monthString},
                                    std::chrono::day{dayString}};

    return Date{ymd};
}

[[nodiscard]] auto Date::operator+(std::chrono::days days) const -> Date {
    auto currentYMD{this->getYMD()};

    return Date{std::chrono::sys_days{currentYMD} + days};
}
}; // namespace hbt::mods
