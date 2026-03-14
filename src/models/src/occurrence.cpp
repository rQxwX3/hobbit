#include <occurrence.hpp>

#include <chrono>

namespace hbt::mods {
Occurrence::Occurrence()
    : weekday_{std::chrono::weekday()}, daypart_{Daypart::MORNING} {}

Occurrence::Occurrence(weekday_t weekday, daypart_t daypart)
    : weekday_{weekday}, daypart_{daypart} {}

[[nodiscard]] auto Occurrence::getWeekday() const -> weekday_t {
    return weekday_;
}

[[nodiscard]] auto Occurrence::getDaypart() const -> daypart_t {
    return daypart_;
}

[[nodiscard]] auto Occurrence::toJSON() const -> nlohmann::json {
    return {{"weekday", weekday_.iso_encoding()}, {"daypart", daypart_}};
}

[[nodiscard]] auto Occurrence::fromJSON(const nlohmann::json &json)
    -> Occurrence {
    auto isoVal{json["weekday"]};

    return {std::chrono::weekday(isoVal), json["daypart"]};
}

[[nodiscard]] auto Occurrence::operator==(const Occurrence &other) const
    -> bool {
    return weekday_ == other.getWeekday() && daypart_ == other.getDaypart();
}
} // namespace hbt::mods
