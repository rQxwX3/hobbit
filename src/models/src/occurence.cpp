#include <occurence.hpp>

#include <chrono>

namespace hbt::mods {
Occurence::Occurence()
    : weekday_{std::chrono::weekday()}, daypart_{Daypart::MORNING} {}

Occurence::Occurence(weekday_t weekday, daypart_t daypart)
    : weekday_{weekday}, daypart_{daypart} {}

[[nodiscard]] auto Occurence::getWeekday() const -> weekday_t {
    return weekday_;
}

[[nodiscard]] auto Occurence::getDaypart() const -> daypart_t {
    return daypart_;
}

[[nodiscard]] auto Occurence::toJSON() const -> nlohmann::json {
    return {{"weekday", weekday_.iso_encoding()}, {"daypart", daypart_}};
}

[[nodiscard]] auto Occurence::fromJSON(const nlohmann::json &json)
    -> Occurence {
    auto isoVal{json["weekday"]};

    return {std::chrono::weekday(isoVal), json["daypart"]};
}

[[nodiscard]] auto Occurence::operator==(const Occurence &other) const -> bool {
    return weekday_ == other.getWeekday() && daypart_ == other.getDaypart();
}
} // namespace hbt::mods
