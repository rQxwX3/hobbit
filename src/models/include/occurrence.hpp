#pragma once

#include <nlohmann/json.hpp>

#include <chrono>

namespace hbt::mods {
class Occurrence {
  private:
    using weekday_t = std::chrono::weekday;

  private:
    weekday_t weekday_;

  public:
    Occurrence();

    Occurrence(weekday_t weekday);

  public:
    [[nodiscard]] auto getWeekday() const -> weekday_t;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> Occurrence;

  public:
    [[nodiscard]] auto operator==(const Occurrence &other) const -> bool;
};
} // namespace hbt::mods
