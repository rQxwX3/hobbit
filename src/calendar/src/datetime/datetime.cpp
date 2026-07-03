#include <datetime/datetime.hpp>

namespace clndr::dt {
using std::chrono::system_clock;

DateTime::DateTime() : value_{DateTime::now().value_} {}

DateTime::DateTime(value_t value) : value_{value} {}

DateTime::DateTime(Date date, Time time)
    : value_{duration_t(date.toDuration() + time.toDuration())} {
    if (!date.ok()) {
        throw std::invalid_argument(
            Error::getMessage(Error::Code::FailedToValidateDate));
    }

    if (!time.ok()) {
        throw std::invalid_argument(
            Error::getMessage(Error::Code::FailedToValidateTime));
    }
}

[[nodiscard]] auto DateTime::getDaysSinceEpoch() const -> Date::duration_t {
    return std::chrono::duration_cast<Date::duration_t>(
        value_.time_since_epoch());
}

[[nodiscard]] auto DateTime::getMinutesSinceMidnight() const
    -> Time::duration_t {
    using namespace std::chrono;

    auto days{floor<std::chrono::days>(value_)};
    auto timeOfDay{value_ - days};

    return duration_cast<Time::duration_t>(timeOfDay);
}

[[nodiscard]] auto DateTime::now() -> DateTime {
    using namespace std::chrono;

    return floor<duration_t>(system_clock::now());
}

[[nodiscard]] auto DateTime::getDate() const -> Date {
    using namespace std::chrono;

    auto ymd{year_month_day(floor<Date::duration_t>(value_))};

    return {static_cast<Date::year_t>(int(ymd.year())),
            static_cast<Date::month_t>(unsigned(ymd.month())),
            static_cast<Date::day_t>(unsigned(ymd.day()))};
}

[[nodiscard]] auto DateTime::getTime() const -> Time {
    auto tod{value_ - std::chrono::floor<std::chrono::days>(value_)};
    auto hms{std::chrono::hh_mm_ss{tod}};

    return {static_cast<Time::hour_t>(hms.hours().count()),
            static_cast<Time::minute_t>(hms.minutes().count())};
}

[[nodiscard]] auto DateTime::getWeekday() const -> constants::Weekday {
    return getDate().getWeekday();
}

[[nodiscard]] auto DateTime::getValue() const -> value_t { return value_; }

[[nodiscard]] auto DateTime::equalDate(DateTime dt1, DateTime dt2) -> bool {
    return dt1.getDaysSinceEpoch() == dt2.getDaysSinceEpoch();
}

[[nodiscard]] auto DateTime::equalTime(DateTime dt1, DateTime dt2) -> bool {
    return dt1.getMinutesSinceMidnight() == dt2.getMinutesSinceMidnight();
}

[[nodiscard]] auto DateTime::operator<=>(const DateTime &other) const
    -> std::strong_ordering = default;

[[nodiscard]] auto DateTime::operator==(const DateTime &other) const
    -> bool = default;

[[nodiscard]] auto DateTime::operator+(const Interval &interval) const
    -> DateTime {
    using namespace std::chrono;
    using Unit = Interval::Unit;

    auto addHoursAndMinutes([](auto &timepoint, Interval::value_t hours,
                               Interval::value_t minutes) -> void {
        timepoint += std::chrono::hours(hours);
        timepoint += std::chrono::minutes(minutes);
    });

    auto dateInDays{floor<days>(value_)};
    auto timeOfDay{value_ - dateInDays};

    auto date{getDate()};
    date = date + interval;

    auto timepoint{sys_days(date.toYMD()) + timeOfDay};
    addHoursAndMinutes(timepoint, interval[Unit::HOUR], interval[Unit::MINUTE]);

    return {timepoint};
}

auto DateTime::operator+=(const Interval &interval) -> DateTime & {
    *this = *this + interval;
    return *this;
}

[[nodiscard]] auto DateTime::diff(const DateTime &dt1, const DateTime &dt2)
    -> Interval {
    using namespace std::chrono;

    auto diff{(dt1 > dt2) ? (dt1.value_ - dt2.value_)
                          : (dt2.value_ - dt1.value_)};

    return Interval::minutes(diff.count());
}

[[nodiscard]] auto DateTime::daysDiff(const DateTime &dt1, const DateTime &dt2)
    -> Interval {
    auto minutesDiff{DateTime::diff(dt1, dt2)[Interval::Unit::MINUTE]};

    return Interval::days(minutesDiff / Interval::minutesInDay);
}
}; // namespace clndr::dt
