#pragma once

#include <nlohmann/json.hpp>

namespace hbt::mods {
enum class Weekday : char {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
};

enum class Daypart : char {
    MORNING,
    AFTERNOON,
    EVENING,
    NIGHT,
};

class Occurence {
  private:
    Weekday weekday_;
    Daypart daypart_;

  public:
    Occurence(Weekday weekday, Daypart daypart);

  public:
    [[nodiscard]] auto getWeekday() const -> Weekday;

    [[nodiscard]] auto getDaypart() const -> Daypart;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json) -> Occurence;
};
} // namespace hbt::mods
