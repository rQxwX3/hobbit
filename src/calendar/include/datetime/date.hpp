#pragma once

#include <chrono>
#include <cstdint>

#include <datetime/constants.hpp>
#include <datetime/interval.hpp>
#include <error.hpp>

namespace clndr::dt {
class Date {
  public:
    using year_t = int16_t;
    using month_t = uint8_t;
    using day_t = uint8_t;

    using duration_t = std::chrono::days;

  public:
    struct Error : core::err::Base<Error> {
        static constexpr auto className{std::string_view{"dt::Date"}};

        enum class Code : uint8_t {
            InvalidInterval,
            InvalidCtorArgs,
        };

        [[nodiscard]] static constexpr auto getMessage(Code code)
            -> std::string {
            switch (code) {
            case Code::InvalidInterval:
                return generateMessage(
                    "cannot perform addition with non-date interval");

            case Code::InvalidCtorArgs:
                return generateMessage(
                    "cannot instantiate valid object from provided arguments");

            default:
                std::unreachable();
            }
        }

        static_assert(core::err::Concept<Error>);
    };

  private:
    /* order must not be changed */
    year_t year_;
    month_t month_;
    day_t day_;

  public:
    Date();

    Date(year_t year, month_t month, day_t day);

  public:
    [[nodiscard]] auto ok() const -> bool;

  public:
    [[nodiscard]] auto next(const Date &date) -> Date;

  public:
    [[nodiscard]] auto getYear() const -> year_t;

    [[nodiscard]] auto getMonth() const -> month_t;

    [[nodiscard]] auto getDay() const -> day_t;

  public:
    [[nodiscard]] auto toDuration() const -> duration_t;

    [[nodiscard]] auto toYMD() const -> std::chrono::year_month_day;

    [[nodiscard]] auto getWeekday() const -> constants::Weekday;

    [[nodiscard]] auto operator<=>(const Date &other) const
        -> std::strong_ordering = default;

    [[nodiscard]] auto operator+(const Interval &interval) const -> Date;

    auto operator+=(const Interval &interval) -> Date &;
};

// static_assert(core::model::Concept<Date>);
} // namespace clndr::dt
