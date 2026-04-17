#include <datetime.hpp>

#include <chrono>
#include <format>
#include <optional>
#include <regex>

namespace hbt::mods {
using std::chrono::floor, std::chrono::system_clock, std::chrono::duration_cast;

DateTime::DateTime()
    : date_{floor<std::chrono::days>(system_clock::now())},

      time_{[]() -> time_t {
          auto now{system_clock::now()};
          auto today{floor<std::chrono::days>(now)};
          return duration_cast<time_t>(now - today);
      }()} {}

DateTime::DateTime(date_t date, time_t time) : date_{date}, time_{time} {}

DateTime::DateTime(year_t year, month_t month, day_t day, hour_t hour,
                   minute_t minute)
    : date_{std::chrono::year_month_day{year, month, day}},
      time_{duration_cast<minute_t>(hour).count() + minute.count()} {}

[[nodiscard]] auto DateTime::getDate() const -> date_t { return date_; }

[[nodiscard]] auto DateTime::today() -> DateTime {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return DateTime(year_month_day(today));
}

[[nodiscard]] auto DateTime::now() -> DateTime {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return DateTime(year_month_day(today), duration_cast<time_t>(now - today));
}

[[nodiscard]] auto DateTime::getYear() const -> year_t { return date_.year(); }

[[nodiscard]] auto DateTime::getMonth() const -> month_t {
    return date_.month();
}

[[nodiscard]] auto DateTime::getDay() const -> day_t { return date_.day(); }

[[nodiscard]] auto DateTime::getWeekday() const -> weekday_t {
    return static_cast<weekday_t>(std::chrono::weekday{date_}.c_encoding());
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

[[nodiscard]] auto DateTime::isToday() const -> bool {
    return DateTime::today().getDate() == date_;
}

[[nodiscard]] auto DateTime::toISO8601String() const -> std::string {
    auto timepoint{std::chrono::sys_days{date_} + time_};
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

    const auto date{date_t(year_t{year}, month_t{month}, day_t{day})};

    if (!date.ok()) {
        return std::nullopt;
    }

    const auto hour{std::stoi(matches[hourGroup].str())};
    const auto minute{std::stoi(matches[minuteGroup].str())};

    if (hour < 0 || hour > DurationUnits::hoursInDay - 1 || minute < 0 ||
        minute > DurationUnits::minutesInHour - 1) {
        return std::nullopt;
    }

    auto time{duration_cast<time_t>(hour_t{hour}) + minute_t{minute}};

    return DateTime{date, time};
}

[[nodiscard]] auto DateTime::operator+(const Interval &interval) const
    -> DateTime {
    auto clampToMonthEnd = [](auto ymd) -> auto {
        return std::chrono::year_month_day_last{
            ymd.year(), std::chrono::month_day_last{ymd.month()}};
    };

    auto newYMD{date_ +
                std::chrono::years{interval.getUnit(Interval::unit_t::YEAR)} +
                std::chrono::months{interval.getUnit(Interval::unit_t::MONTH)}};

    if (Interval::MonthHandling::CUT_OFF == interval.getMonthHandling() &&
        !newYMD.ok()) {
        newYMD = clampToMonthEnd(newYMD);
    }

    auto newSysDays{std::chrono::sys_days{newYMD}};
    newSysDays += std::chrono::weeks{interval.getUnit(Interval::unit_t::WEEK)} +
                  std::chrono::days{interval.getUnit(Interval::unit_t::DAY)};

    return DateTime{newSysDays};
}

auto DateTime::operator+=(const Interval &interval) -> DateTime & {
    *this = *this + interval;
    return *this;
}
}; // namespace hbt::mods
