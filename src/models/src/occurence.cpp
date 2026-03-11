#include <occurence.hpp>

namespace hbt::mods {
Occurence::Occurence()
    : weekday_{Weekday::MONDAY}, daypart_{Daypart::MORNING} {}

Occurence::Occurence(Weekday weekday, Daypart daypart)
    : weekday_{weekday}, daypart_{daypart} {}

[[nodiscard]] auto Occurence::getWeekday() const -> Weekday { return weekday_; }

[[nodiscard]] auto Occurence::getDaypart() const -> Daypart { return daypart_; }

[[nodiscard]] auto Occurence::toJSON() const -> nlohmann::json {
    return {{"weekday", weekday_}, {"daypart", daypart_}};
}

[[nodiscard]] auto Occurence::fromJSON(const nlohmann::json &json)
    -> Occurence {
    return {json["weekday"], json["daypart"]};
}
} // namespace hbt::mods
