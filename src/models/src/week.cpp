#include <algorithm>
#include <week.hpp>

namespace hbt::mods {
auto Week::validateAndReverseString(std::string string) -> std::string {
    if (string.length() != weekdaysCount) {
        throw std::invalid_argument(
            errorMessage(Error::StringLengthExceedsLimit));
    }

    std::ranges::reverse(string);
    return string;
}

Week::Week(const std::vector<Week::Weekday> &weekdays) {
    for (const auto wd : weekdays) {
        days_.set(static_cast<size_t>(wd));
    }
}

Week::Week(days_t days) : days_{days} {}

[[nodiscard]] auto Week::getDays() const -> days_t { return days_; }

[[nodiscard]] auto Week::containsWeekday(Week::Weekday weekday) const -> bool {
    return days_.test(static_cast<size_t>(weekday));
}

[[nodiscard]] auto Week::isEmpty() const -> bool {
    return std::ranges::none_of(
        Week::weekdays.begin(), Week::weekdays.end(),
        [this](const auto wd) -> bool { return this->containsWeekday(wd); });
}

[[nodiscard]] auto Week::toJSON() const -> nlohmann::json {
    return days_.to_string();
}

[[nodiscard]] auto Week::fromJSON(const nlohmann::json &json)
    -> std::expected<Week, Error> {
    if (!json.is_string()) {
        return std::unexpected(Error::JSONNotString);
    }

    try {
        auto daysFromString{
            days_t(validateAndReverseString(json.get<std::string>()))};

        return Week(daysFromString);
    } catch (std::invalid_argument) {
        return std::unexpected(Error::JSONInvalidString);
    }
}
}; // namespace hbt::mods
