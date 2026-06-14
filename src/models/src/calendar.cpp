#include <calendar.hpp>
#include <interval.hpp>
#include <week.hpp>

namespace hbt::mods::clndr {
Week::Week(mods::DateTime dt) {
    for (auto i{0}; i != mods::Week::weekdaysCount; ++i) {
        array_[static_cast<size_t>(dt.getWeekday())] = dt;
        dt += Interval::days(1);
    }
}

Week::Week(array_t array) : array_{array} {}

[[nodiscard]] auto Week::operator[](mods::Week::Weekday wd) -> DateTime & {
    return array_[static_cast<size_t>(wd)];
}

[[nodiscard]] auto Week::operator[](mods::Week::Weekday wd) const -> DateTime {
    return array_[static_cast<size_t>(wd)];
}

[[nodiscard]] auto Week::toJSON() const -> nlohmann::json {
    auto json = nlohmann::json{{}};

    for (auto i{0}; i != mods::Week::weekdaysCount; ++i) {
        json[i] = array_[i].toISO8601String();
    }

    return json;
}

[[nodiscard]] auto Week::fromJSON(const nlohmann::json &json)
    -> std::expected<Week, Error> {
    auto array{array_t{}};

    for (auto wd : mods::Week::weekdays) {
        auto dtFromJSON{
            DateTime::fromISO8601String(json[static_cast<size_t>(wd)])};
        if (!dtFromJSON) {
            return std::unexpected(Error::JSONFailedtoParseDateTime);
        }

        array[static_cast<size_t>(wd)] = dtFromJSON.value();
    }

    return Week(array);
}
}; // namespace hbt::mods::clndr
