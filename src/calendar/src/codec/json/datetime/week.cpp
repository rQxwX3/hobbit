#include <codec/iso8601/date.hpp>
#include <codec/json/datetime/week.hpp>
#include <datetime/constants.hpp>

namespace clndr::codec::json::dt {
[[nodiscard]] auto Week::encode(const clndr::dt::Week &week) -> nlohmann::json {
    auto json = nlohmann::json{{}};
    auto array{week.toArray()};

    for (auto i{0}; i != clndr::dt::constants::weekdaysCount; ++i) {
        json[i] = iso8601::Date::encode(array[i]);
    }

    return json;
}

[[nodiscard]] auto Week::decode(const nlohmann::json &json)
    -> std::expected<clndr::dt::Week, Error::Code> {
    auto array{clndr::dt::Week::array_t{}};

    for (auto wd : clndr::dt::constants::weekdaysArray) {
        auto dateFromJSON{iso8601::Date::decode(json[static_cast<size_t>(wd)])};
        if (!dateFromJSON) {
            return std::unexpected(Error::Code::FailedToParseDate);
        }

        array[static_cast<size_t>(wd)] = dateFromJSON.value();
    }

    return clndr::dt::Week(array);
}
} // namespace clndr::codec::json::dt
