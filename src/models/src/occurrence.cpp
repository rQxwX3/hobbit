#include <occurrence.hpp>

namespace hbt::mods {
Occurrence::Occurrence() : date_{Date{}}, interval_{std::nullopt} {}

Occurrence::Occurrence(hbt::mods::Date date)
    : date_{date}, interval_{std::nullopt} {}

Occurrence::Occurrence(hbt::mods::Date date, interval_t interval)
    : date_{date}, interval_{interval} {}

[[nodiscard]] auto Occurrence::getDate() const -> hbt::mods::Date {
    return date_;
}

[[nodiscard]] auto Occurrence::getInterval() const -> interval_t {
    return interval_;
}

[[nodiscard]] auto Occurrence::getWeekday() const
    -> hbt::mods::Date::weekday_t {
    return date_.getWeekday();
}

[[nodiscard]] auto Occurrence::toJSON() const -> nlohmann::json {
    auto intervalJSON{(interval_.has_value()) ? interval_->toJSON() : "none"};

    return {{"date", date_.toYMDString()}, {"interval", intervalJSON}};
}

[[nodiscard]] auto Occurrence::fromJSON(const nlohmann::json &json)
    -> std::optional<Occurrence> {
    if (!json.contains("date") || !json.contains("interval")) {
        return std::nullopt;
    }

    return Occurrence{
        hbt::mods::Date::fromYMDString(json["date"].get<std::string>()),
        hbt::mods::Interval::fromJSON(json["interval"])};
}

[[nodiscard]] auto Occurrence::isForDate(Date date) const -> bool {
    if (!interval_.has_value() && date_ != date) {
        return false;
    }

    for (auto dateCopy{date_}; dateCopy < date; dateCopy += interval_.value()) {
        if (dateCopy == date) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] auto Occurrence::operator==(const Occurrence &other) const
    -> bool {
    return date_ == date_ &&
           Occurrence::compareOptional(interval_, other.interval_);
}
} // namespace hbt::mods
