#include <algorithm>
#include <weekdays.hpp>

namespace hbt::mods {
auto Weekdays::validateAndReverseString(std::string string) -> std::string {
    if (string.length() != daysCount) {
        throw std::invalid_argument(
            errorMessage(Error::StringLengthMoreThanWeek));
    }

    std::ranges::reverse(string);
    return string;
}

Weekdays::Weekdays(const std::vector<Week> &weekdays) {
    for (const auto wd : weekdays) {
        days_.set(static_cast<size_t>(wd));
    }
}

Weekdays::Weekdays(days_t days) : days_{days} {}

[[nodiscard]] auto Weekdays::getDays() const -> days_t { return days_; }

[[nodiscard]] auto Weekdays::containsWeekday(Week weekday) const -> bool {
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

    try {
        auto daysFromString{
            days_t(validateAndReverseString(json.get<std::string>()))};

        return Weekdays(daysFromString);
    } catch (std::invalid_argument) {
        return std::unexpected(Error::JSONInvalidString);
    }
}
}; // namespace hbt::mods
