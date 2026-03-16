#pragma once

#include <nlohmann/json.hpp>

#include <chrono>

namespace hbt::mods {
class Date {
  private:
    std::chrono::year_month_day ymd_;

  private:
    [[nodiscard]] auto getYMD() const -> std::chrono::year_month_day;

  public:
    explicit Date(std::chrono::year_month_day ymd);

  public:
    [[nodiscard]] static auto today() -> Date;

  public:
    [[nodiscard]] auto getYear() const -> std::chrono::year;

    [[nodiscard]] auto getMonth() const -> std::chrono::month;

    [[nodiscard]] auto getDay() const -> std::chrono::day;

    [[nodiscard]] auto getWeekday() const -> std::chrono::weekday;

  public:
    [[nodiscard]] auto operator<=>(const Date &other) const -> bool = default;

  public:
    [[nodiscard]] auto isToday() const -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json) -> Date;
};
} // namespace hbt::mods
