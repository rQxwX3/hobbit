#include <occurrence.hpp>

#include <chrono>

namespace hbt::mods {
Occurrence::Occurrence() : date_{Date{}}, intervalDays_{0} {}

Occurrence::Occurrence(hbt::mods::Date date) : date_{date}, intervalDays_{0} {}

Occurrence::Occurrence(hbt::mods::Date date, std::chrono::days intervalDays)
    : date_{date}, intervalDays_{intervalDays} {}

Occurrence::Occurrence(hbt::mods::Date date, int intervalDays)
    : date_{date}, intervalDays_{intervalDays} {}

[[nodiscard]] auto Occurrence::getDate() const -> hbt::mods::Date {
    return date_;
}

[[nodiscard]] auto Occurrence::getWeekday() const -> std::chrono::weekday {
    return date_.getWeekday();
}

[[nodiscard]] auto Occurrence::getIntervalDays() const -> std::chrono::days {
    return intervalDays_;
}

[[nodiscard]] auto Occurrence::toJSON() const -> nlohmann::json {
    return {{"date", date_.toYMDString()},
            {"interval_days", intervalDays_.count()}};
}

[[nodiscard]] auto Occurrence::fromJSON(const nlohmann::json &json)
    -> Occurrence {
    if (!json.contains("date") || !json.contains("interval_days")) {
        throw std::runtime_error("Missing required fields OCCURRENCE");
    }

    return Occurrence{
        hbt::mods::Date::fromYMDString(json["date"].get<std::string>()),
        json["interval_days"].get<int>()};
}

[[nodiscard]] auto Occurrence::isForDate(Date date) const -> bool {
    auto ymdDiff{std::chrono::sys_days{date.getYMD()} -
                 std::chrono::sys_days{date_.getYMD()}};

    if (ymdDiff.count() < 0) {
        return false;
    }

    return ymdDiff.count() % intervalDays_.count() == 0;
}
} // namespace hbt::mods
