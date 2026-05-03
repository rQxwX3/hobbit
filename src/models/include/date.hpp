#pragma once

#include <interval.hpp>
#include <ymd.hpp>

#include <chrono>

namespace hbt::mods {
class Date {
  public:
    using ymd_t = hbt::mods::YMD;

    using year_t = ymd_t::year_t;
    using month_t = ymd_t::month_t;
    using day_t = ymd_t::day_t;

    using weekday_t = ymd_t::weekday_t;

  public:
    enum class Error : uint8_t {
        InvalidYMD,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidYMD:
            return "Date: provided YMD value is not valid";

        default:
            std::unreachable();
        }
    }

  private:
    static inline auto clampToMonthEnd{[](ymd_t ymd) -> auto {
        using namespace std::chrono;

        if (ymd.ok()) {
            return ymd;
        }

        auto ymdToChrono{ymd.toChrono()};

        auto res{year_month_day{year_month_day_last{
            ymdToChrono.year(), month_day_last{ymdToChrono.month()}}}};
        assert(res.ok());

        auto ymdFromChrono{YMD::fromChrono(res)};
        assert(ymdFromChrono);
        assert(ymdFromChrono->ok());

        return ymdFromChrono.value();
    }};

    static inline auto resolveMonthOverflow{[](ymd_t ymd) -> auto {
        using namespace std::chrono;

        if (ymd.ok()) {
            return ymd;
        }

        auto ymdToChrono{ymd.toChrono()};

        auto lastDay{year_month_day_last{ymdToChrono.year(),
                                         month_day_last{ymdToChrono.month()}}};

        auto overflow{static_cast<int>(unsigned(ymdToChrono.day())) -
                      static_cast<int>(unsigned(lastDay.day()))};

        auto res{year_month_day{sys_days{lastDay} + days{overflow}}};
        assert(res.ok());

        auto ymdFromChrono{YMD::fromChrono(res)};
        assert(ymdFromChrono);
        assert(ymdFromChrono->ok());

        return ymdFromChrono.value();
    }};

  private:
    ymd_t YMD_;

  private:
    [[nodiscard]] static auto ymdValidator(YMD ymd) -> ymd_t;

  public:
    Date();

    Date(ymd_t ymd);

    Date(year_t year, month_t month, day_t day);

  public:
    [[nodiscard]] auto getYMD() const -> ymd_t;

    [[nodiscard]] auto getChronoYMD() const -> std::chrono::year_month_day;

    [[nodiscard]] auto getWeekday() const -> weekday_t;

    [[nodiscard]] auto getYear() const -> year_t;

    [[nodiscard]] auto getMonth() const -> month_t;

    [[nodiscard]] auto getDay() const -> day_t;

  public:
    [[nodiscard]] static auto today() -> Date;

  public:
    [[nodiscard]] auto isToday() const -> bool;

  public:
    [[nodiscard]] auto operator<=>(const Date &other) const
        -> std::strong_ordering;

    [[nodiscard]] auto operator==(const Date &other) const -> bool;

    [[nodiscard]] auto operator!=(const Date &other) const -> bool;

    [[nodiscard]] auto operator+(const Interval &interval) const -> Date;

    [[nodiscard]] auto operator-(const Interval &interval) const -> Date;

    auto operator+=(const Interval &interval) -> Date &;

    auto operator-=(const Interval &interval) -> Date &;

  public:
    [[nodiscard]] static auto diff(const Date &first, const Date &second)
        -> Duration;
};
} // namespace hbt::mods
