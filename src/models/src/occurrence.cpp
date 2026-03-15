#include <occurrence.hpp>

#include <chrono>

namespace hbt::mods {
Occurrence::Occurrence() : weekday_{std::chrono::weekday()} {}

Occurrence::Occurrence(weekday_t weekday) : weekday_{weekday} {}

[[nodiscard]] auto Occurrence::getWeekday() const -> weekday_t {
    return weekday_;
}

[[nodiscard]] auto Occurrence::toJSON() const -> nlohmann::json {
    return {{"weekday", weekday_.iso_encoding()}};
}

[[nodiscard]] auto Occurrence::fromJSON(const nlohmann::json &json)
    -> Occurrence {
    if (!json.contains("weekday")) {
        throw std::runtime_error("Missing required fields");
    }

    auto isoVal{json["weekday"]};
    return {std::chrono::weekday(isoVal)};
}

[[nodiscard]] auto Occurrence::operator==(const Occurrence &other) const
    -> bool {
    return weekday_ == other.getWeekday();
}
} // namespace hbt::mods
