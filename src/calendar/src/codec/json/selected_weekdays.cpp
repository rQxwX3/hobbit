#include <codec/json/selected_weekdays.hpp>

namespace clndr::codec::json {
[[nodiscard]] auto
SelectedWeekdays::encode(const rec::SelectedWeekdays &selectedWeekdays)
    -> nlohmann::json {
    return selectedWeekdays.getDays().to_string();
}

[[nodiscard]] auto SelectedWeekdays::decode(const nlohmann::json &json)
    -> std::expected<rec::SelectedWeekdays, Error::Code> {
    if (!json.is_string()) {
        return std::unexpected(Error::Code::NotString);
    }

    try {
        auto daysFromString{rec::SelectedWeekdays::days_t(
            rec::SelectedWeekdays::validateAndReverseString(
                json.get<std::string>()))};

        return rec::SelectedWeekdays(daysFromString);
    } catch (std::invalid_argument) {
        return std::unexpected(Error::Code::InvalidString);
    }
}
}; // namespace clndr::codec::json
