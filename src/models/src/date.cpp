#include <date.hpp>

#include <chrono>

namespace hbt::mods {
Date::Date(std::chrono::year_month_day ymd) : ymd_{ymd} {}

[[nodiscard]] auto Date::today() -> Date {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return year_month_day(today);
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
    return Date::today() == ymd_;
}
}; // namespace hbt::mods
