#pragma once

#include <nlohmann/json.hpp>

#include <chrono>

namespace hbt::mods {
class Date {
  private:
    std::chrono::year_month_day ymd_;

  public:
    Date();

    explicit Date(std::chrono::year_month_day ymd);

    explicit Date(std::chrono::year year, std::chrono::month month,
                  std::chrono::day day);

  public:
    [[nodiscard]] static auto today() -> Date;

  public:
    [[nodiscard]] auto getYear() const -> std::chrono::year;

    [[nodiscard]] auto getMonth() const -> std::chrono::month;

    [[nodiscard]] auto getDay() const -> std::chrono::day;

    [[nodiscard]] auto getYMD() const -> std::chrono::year_month_day;

    [[nodiscard]] auto getWeekday() const -> std::chrono::weekday;

  public:
    [[nodiscard]] auto operator<=>(const Date &other) const -> bool = default;

  public:
    [[nodiscard]] auto isToday() const -> bool;

  public:
    [[nodiscard]] auto toYMDString() const -> std::string;

    [[nodiscard]] static auto fromYMDString(const std::string &string) -> Date;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json) -> Date;

  public:
    template <typename Rep, typename Period>
    [[nodiscard]] auto
    operator+(std::chrono::duration<Rep, Period> interval) const -> Date {
        auto currentYMD{this->getYMD()};

        return Date{std::chrono::sys_days{currentYMD} + interval};
    }
};
} // namespace hbt::mods
