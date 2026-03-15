#pragma once

#include <nlohmann/json.hpp>

#include <date.hpp>

#include <chrono>

namespace hbt::mods {
class Occurrence {
  private:
    hbt::mods::Date date_;

  public:
    Occurrence();

    Occurrence(hbt::mods::Date date);

  public:
    [[nodiscard]] auto getDate() const -> hbt::mods::Date;

    [[nodiscard]] auto getWeekday() const -> std::chrono::weekday;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> Occurrence;

  public:
    [[nodiscard]] auto operator==(const Occurrence &other) const -> bool;
};
} // namespace hbt::mods
