#pragma once

#include <nlohmann/json.hpp>

#include <chrono>

namespace hbt::mods {
enum class Daypart : char {
    MORNING,
    AFTERNOON,
    EVENING,
    NIGHT,
};

class Occurrence {
  private:
    using daypart_t = Daypart;
    using weekday_t = std::chrono::weekday;

  private:
    weekday_t weekday_;
    daypart_t daypart_;

  public:
    Occurrence();

    Occurrence(weekday_t weekday, daypart_t daypart);

  public:
    [[nodiscard]] auto getWeekday() const -> weekday_t;

    [[nodiscard]] auto getDaypart() const -> daypart_t;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> Occurrence;

  public:
    [[nodiscard]] auto operator==(const Occurrence &other) const -> bool;
};
} // namespace hbt::mods
