#include <weekdays.hpp>

namespace hbt::mods {
Weekdays::Weekdays(const std::vector<DateTime::weekday_t> &weekdays) {
    for (const auto wd : weekdays) {
        days_.set(static_cast<size_t>(wd));
    }
}

Weekdays::Weekdays(days_t days) : days_{days} {}

Weekdays::Weekdays(const std::string &daysString) : days_{daysString} {}

[[nodiscard]] auto Weekdays::getDays() const -> days_t { return days_; }

[[nodiscard]] auto Weekdays::containsWeekday(weekday_t weekday) const -> bool {
    return days_.test(static_cast<size_t>(weekday));
}

[[nodiscard]] auto Weekdays::toJSON() const -> nlohmann::json {
    return days_.to_string();
}

[[nodiscard]] auto Weekdays::fromJSON(const nlohmann::json &json)
    -> std::optional<Weekdays> {
    if (!json.is_string()) {
        return std::nullopt;
    }

    auto str{json.get<std::string>()};
    if (str.length() != daysCount) {
        return std::nullopt;
    }

    try {
        return Weekdays{str};
    } catch (std::invalid_argument) {
        return std::nullopt;
    }
}
}; // namespace hbt::mods
