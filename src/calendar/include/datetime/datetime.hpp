#pragma once

#include <chrono>

#include <nlohmann/json.hpp>

#include <datetime/constants.hpp>
#include <datetime/date.hpp>
#include <datetime/interval.hpp>
#include <datetime/time.hpp>

#include <error.hpp>

namespace clndr::dt {
class DateTime {
  public:
    using duration_t = std::chrono::minutes;
    using value_t = std::chrono::sys_time<duration_t>;

  public:
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"dt::DateTime"}};

        enum class Code : uint8_t {
            InvalidCtorArgs,

            FailedToValidateDate,
            FailedToValidateTime,

            ISO8601RegexMismatch,
            ISO8601UnitNotMatched,

            ISO8601InvalidDate,
            ISO8601InvalidTime,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::InvalidCtorArgs:
                return generateMessage(
                    "cannot instantiate valid object from provided arguments");

            case Code::FailedToValidateDate:
                return generateMessage("failed to validate provided Date");

            case Code::FailedToValidateTime:
                return generateMessage("failed to validate provided Time");

            case Code::ISO8601RegexMismatch:
                return generateMessage("provided input doesn't match regex");

            case Code::ISO8601UnitNotMatched:
                return generateMessage(
                    "provided input doesn't contain required unit(s)");

            case Code::ISO8601InvalidDate:
                return generateMessage("provided input contains invalid Date");

            case Code::ISO8601InvalidTime:
                return generateMessage("provided input contains invalid Time");

            default:
                std::unreachable();
            }
        }
    };

    static_assert(core::err::Concept<Error>);

  private:
    value_t value_;

  public:
    [[nodiscard]] auto ok() const -> bool;

  private:
    [[nodiscard]] auto getDaysSinceEpoch() const -> Date::duration_t;

    [[nodiscard]] auto getMinutesSinceMidnight() const -> Time::duration_t;

  public:
    DateTime();

    DateTime(value_t value);

    DateTime(Date date, Time time = Time::midnight());

  public:
    [[nodiscard]] static auto now() -> DateTime;

  public:
    [[nodiscard]] auto getDate() const -> Date;

    [[nodiscard]] auto getTime() const -> Time;

    [[nodiscard]] auto getWeekday() const -> constants::Weekday;

    [[nodiscard]] auto getValue() const -> value_t;

  public:
    [[nodiscard]] static auto equalDate(DateTime dt1, DateTime dt2) -> bool;

    [[nodiscard]] static auto equalTime(DateTime dt1, DateTime dt2) -> bool;

  public:
    [[nodiscard]] auto operator<=>(const DateTime &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const DateTime &other) const -> bool;

    [[nodiscard]] auto operator+(const Interval &interval) const -> DateTime;

    auto operator+=(const Interval &interval) -> DateTime &;

  public:
    [[nodiscard]] static auto diff(const DateTime &dt1, const DateTime &dt2)
        -> Interval;

    [[nodiscard]] static auto daysDiff(const DateTime &dt1, const DateTime &dt2)
        -> Interval;
};
} // namespace clndr::dt
