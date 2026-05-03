#include <ymd.hpp>

#include <cassert>

namespace hbt::mods {
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
}; // namespace hbt::mods
