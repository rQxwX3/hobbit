#pragma once

#include <duration.hpp>

#include <cassert>
#include <chrono>
#include <cstdint>
#include <expected>
#include <string>
#include <utility>

namespace hbt::mods {
class YMD {
  public:
    /* types below reflect value ranges for corresponding std::chrono types */
    using year_t = int16_t;
    using month_t = uint8_t;
    using day_t = uint8_t;

  public:
    enum class weekday_t : uint8_t {
        SUNDAY = 0,
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        COUNT_,
    };

  public:
    enum class Error : uint8_t {
        InvalidArguments,

        InvalidChrono,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidArguments:
            return "YMD: cannot construct valid YMD object from provided "
                   "values";

        case Error::InvalidChrono:
            return "YMD: provided std::chrono::year_month_day value is not "
                   "valid";

        default:
            std::unreachable();
        }
    }

  private:
    /* the order must not be changed */
    year_t year_;
    month_t month_;
    day_t day_;

  public:
    [[nodiscard]] auto toChrono() const -> std::chrono::year_month_day;

    [[nodiscard]] static auto
    fromChrono(const std::chrono::year_month_day &chrono)
        -> std::expected<YMD, Error>;

  public:
    YMD(year_t year, month_t month, day_t day);

  public:
    [[nodiscard]] auto operator<=>(const YMD &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const YMD &other) const -> bool;

  public:
    [[nodiscard]] static auto today() -> YMD;

  public:
    [[nodiscard]] auto ok() const -> bool;

  public:
    [[nodiscard]] auto getYear() const -> year_t;

    [[nodiscard]] auto getMonth() const -> month_t;

    [[nodiscard]] auto getDay() const -> day_t;

    [[nodiscard]] auto getWeekday() const -> weekday_t;
};

} // namespace hbt::mods
