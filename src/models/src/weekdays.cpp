#include <weekdays.hpp>

namespace hbt::mods {
auto Weekdays::validateString(const std::string &string) -> std::string {
    if (string.length() != daysCount) {
        throw std::invalid_argument(
            errorMessage(Error::StringLengthMoreThanWeek));
    }

    return string;
}

Weekdays::Weekdays(const std::vector<DateTime::weekday_t> &weekdays) {
    for (const auto wd : weekdays) {
        days_.set(static_cast<size_t>(wd));
    }
}

Weekdays::Weekdays(days_t days) : days_{days} {}

Weekdays::Weekdays(const std::string &daysString)
    : days_{validateString(daysString)} {}

[[nodiscard]] auto Weekdays::getDays() const -> days_t { return days_; }

[[nodiscard]] auto Weekdays::containsWeekday(weekday_t weekday) const -> bool {
    return days_.test(static_cast<size_t>(weekday));
}

[[nodiscard]] auto Weekdays::toJSON() const -> nlohmann::json {
    return days_.to_string();
}

[[nodiscard]] auto Weekdays::fromJSON(const nlohmann::json &json)
    -> std::expected<Weekdays, Error> {
    if (!json.is_string()) {
        return std::unexpected(Error::JSONNotString);
    }

    return Weekdays{validateString(json.get<std::string>())};
}
}; // namespace hbt::mods
