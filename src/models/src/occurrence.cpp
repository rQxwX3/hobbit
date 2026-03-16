#include <occurrence.hpp>

#include <chrono>

namespace hbt::mods {
Occurrence::Occurrence(hbt::mods::Date date) : date_{date} {}

[[nodiscard]] auto Occurrence::getWeekday() const -> std::chrono::weekday {
    return date_.getWeekday();
}

[[nodiscard]] auto Occurrence::toJSON() const -> nlohmann::json {
    return {{"date", date_.toJSON()}};
}

[[nodiscard]] auto Occurrence::fromJSON(const nlohmann::json &json)
    -> Occurrence {
    if (!json.contains("date")) {
        throw std::runtime_error("Missing required fields");
    }

    return Occurrence{hbt::mods::Date::fromJSON(json["date"])};
}

[[nodiscard]] auto Occurrence::getDate() const -> hbt::mods::Date {
    return date_;
}

[[nodiscard]] auto Occurrence::operator==(const Occurrence &other) const
    -> bool {
    return date_ == other.getDate();
}
} // namespace hbt::mods
