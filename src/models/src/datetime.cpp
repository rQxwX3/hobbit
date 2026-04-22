#include <date.hpp>
#include <datetime.hpp>

#include <format>
#include <optional>
#include <regex>

namespace hbt::mods {
using std::chrono::system_clock;

DateTime::DateTime()
    : date_{floor<std::chrono::days>(system_clock::now())},

      time_{[]() -> time_t {
          auto now{system_clock::now()};
          auto today{floor<std::chrono::days>(now)};
          return duration_cast<time_t>(now - today);
      }()} {}

DateTime::DateTime(mods::Date date, time_t time) : date_{date}, time_{time} {}

DateTime::DateTime(year_t year, month_t month, day_t day, hour_t hour,
                   minute_t minute)
    : date_{year, month, day},
      time_{duration_cast<minute_t>(hour).count() + minute.count()} {}

[[nodiscard]] auto DateTime::getDate() const -> mods::Date { return date_; }

[[nodiscard]] auto DateTime::now() -> DateTime {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return DateTime(mods::Date::today(), duration_cast<time_t>(now - today));
}

[[nodiscard]] auto DateTime::getTime() const -> time_t { return time_; }

[[nodiscard]] auto DateTime::getHour() const -> hour_t {
    auto hms{std::chrono::hh_mm_ss<time_t>(time_)};

    return hms.hours();
}

[[nodiscard]] auto DateTime::getMinute() const -> minute_t {
    auto hms{std::chrono::hh_mm_ss<time_t>(time_)};

    return hms.minutes();
}

[[nodiscard]] auto DateTime::equalDates(DateTime dt1, DateTime dt2) -> bool {
    return dt1.getDate() == dt2.getDate();
}

[[nodiscard]] auto DateTime::toISO8601String() const -> std::string {
    auto timepoint{std::chrono::sys_days{date_.getYMD()} + time_};
    return std::format("{:%Y-%m-%dT%H:%M}", timepoint);
}

[[nodiscard]] auto DateTime::fromISO8601String(const std::string &string)
    -> std::optional<DateTime> {
    const auto pattern{std::regex{
        R"(^(\d{4})[-./](\d{2})[-./](\d{2})[ T](\d{2}):(\d{2})(?::(\d{2}))?$)"}};

    constexpr size_t yearGroup{1};
    constexpr size_t monthGroup{2};
    constexpr size_t dayGroup{3};
    constexpr size_t hourGroup{4};
    constexpr size_t minuteGroup{5};
    constexpr size_t secondGroup{6}; // not supported, probably never will be

    std::smatch matches;
    if (!std::regex_match(string, matches, pattern)) {
        return std::nullopt;
    }

    if (!matches[yearGroup].matched || !matches[monthGroup].matched ||
        !matches[dayGroup].matched || !matches[hourGroup].matched ||
        !matches[minuteGroup].matched) {
        return std::nullopt;
    }

    const auto year{std::stoi(matches[yearGroup].str())};
    const auto month{
        static_cast<unsigned>(std::stoi(matches[monthGroup].str()))};
    const auto day{static_cast<unsigned>(std::stoi(matches[dayGroup].str()))};

    const auto date{mods::Date(year_t{year}, month_t{month}, day_t{day})};

    const auto hour{std::stoi(matches[hourGroup].str())};
    const auto minute{std::stoi(matches[minuteGroup].str())};

    if (hour < 0 || hour > DurationUnits::hoursInDay - 1 || minute < 0 ||
        minute > DurationUnits::minutesInHour - 1) {
        return std::nullopt;
    }

    auto time{duration_cast<time_t>(hour_t{hour}) + minute_t{minute}};

    return DateTime{date, time};
}

[[nodiscard]] auto DateTime::operator<=>(const DateTime &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto DateTime::operator==(const DateTime &other) const
    -> bool = default;

[[nodiscard]] auto DateTime::operator+(const Interval &interval) const
    -> DateTime {
    // auto newTime{getTime() + interval}; TODO: add time too, check for
    // overflow, add another day to the interval if it happens
    auto newDate{getDate() + interval};

    return DateTime{};
}

auto DateTime::operator+=(const Interval &interval) -> DateTime & {
    *this = *this + interval;
    return *this;
}

[[nodiscard]] auto DateTime::operator-(const DateTime &other) const
    -> DurationUnits {
    auto dateDiff{getDate() - other.getDate()};
    auto timeDiff{getTime() - other.getTime()};

    return dateDiff + DurationUnits::fromUnit(DurationUnits::unit_t::MINUTE,
                                              timeDiff.count());
}
}; // namespace hbt::mods
