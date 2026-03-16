#pragma once

#include <nlohmann/json.hpp>

#include <date.hpp>

#include <chrono>

namespace hbt::mods {
class Occurrence {
  private:
    hbt::mods::Date date_;

    std::chrono::days intervalDays_;

  public:
    Occurrence();

    Occurrence(hbt::mods::Date date);

    Occurrence(hbt::mods::Date date, std::chrono::days intervalDays);

    Occurrence(hbt::mods::Date date, int intervalDays);

  public:
    [[nodiscard]] auto getDate() const -> hbt::mods::Date;

    [[nodiscard]] auto getWeekday() const -> std::chrono::weekday;

    [[nodiscard]] auto getIntervalDays() const -> std::chrono::days;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> Occurrence;

  public:
    [[nodiscard]] auto operator<=>(const Occurrence &other) const
        -> bool = default;
};
} // namespace hbt::mods
