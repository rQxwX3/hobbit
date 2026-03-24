#include <occurrence.hpp>

namespace hbt::mods {
Occurrence::Occurrence(hbt::mods::Date date, hbt::mods::Interval interval)
    : date_{date}, interval_{interval} {}

[[nodiscard]] auto Occurrence::getDate() const -> hbt::mods::Date {
    return date_;
}

[[nodiscard]] auto Occurrence::getInterval() const -> hbt::mods::Interval {
    return interval_;
}

[[nodiscard]] auto Occurrence::getWeekday() const
    -> hbt::mods::Date::weekday_t {
    return date_.getWeekday();
}

[[nodiscard]] auto Occurrence::toJSON() const -> nlohmann::json {
    return {{"date", date_.toISO8601String()},
            {"interval", interval_.toJSON()}};
}

[[nodiscard]] auto Occurrence::fromJSON(const nlohmann::json &json)
    -> std::optional<Occurrence> {
    if (!json.contains("date") || !json.contains("interval")) {
        return std::nullopt;
    }

    auto dateFromISO8601String{
        hbt::mods::Date::fromISO8601String(json["date"].get<std::string>())};
    if (!dateFromISO8601String.has_value()) {
        return std::nullopt;
    }

    auto intervalFromJSON{hbt::mods::Interval::fromJSON(json["interval"])};
    if (!intervalFromJSON.has_value()) {
        return std::nullopt;
    }

    return Occurrence{dateFromISO8601String.value(), intervalFromJSON.value()};
}

[[nodiscard]] auto Occurrence::isForDate(Date date) const -> bool {
    if (interval_.isZero() && date_ != date) {
        return false;
    }

    for (auto dateCopy{date_}; dateCopy <= date; dateCopy += interval_) {
        if (dateCopy == date) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto Occurrence::operator==(const Occurrence &other) const
    -> bool {
    return date_ == date_ && interval_ == other.interval_;
}
} // namespace hbt::mods
