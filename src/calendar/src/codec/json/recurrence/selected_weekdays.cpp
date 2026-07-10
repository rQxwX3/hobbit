#include <codec/json/recurrence/selected_weekdays.hpp>

namespace clndr::codec::json::rec {
[[nodiscard]] auto
SelectedWeekdays::encode(const clndr::rec::SelectedWeekdays &selectedWeekdays)
    -> nlohmann::json {
    return selectedWeekdays.getDays().to_string();
}

[[nodiscard]] auto SelectedWeekdays::decode(const nlohmann::json &json)
    -> std::expected<clndr::rec::SelectedWeekdays, Error::Code> {
    if (!json.is_string()) {
        return std::unexpected(Error::Code::NotString);
    }

    try {
        auto daysFromString{clndr::rec::SelectedWeekdays::days_t(
            clndr::rec::SelectedWeekdays::validateAndReverseString(
                json.get<std::string>()))};

        return clndr::rec::SelectedWeekdays(daysFromString);
    } catch (std::invalid_argument) {
        return std::unexpected(Error::Code::InvalidString);
    }
}
}; // namespace clndr::codec::json::rec
