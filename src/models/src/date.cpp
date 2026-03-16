#include <date.hpp>

#include <chrono>
#include <format>

namespace hbt::mods {
Date::Date(std::chrono::year_month_day ymd) : ymd_{ymd} {}

[[nodiscard]] auto Date::getYMD() const -> std::chrono::year_month_day {
    return ymd_;
}

[[nodiscard]] auto Date::today() -> Date {
    using namespace std::chrono;

    auto now{system_clock::now()};
    auto today{floor<days>(now)};

    return Date(year_month_day(today));
}

[[nodiscard]] auto Date::getYear() const -> std::chrono::year {
    return ymd_.year();
}

[[nodiscard]] auto Date::getMonth() const -> std::chrono::month {
    return ymd_.month();
}

[[nodiscard]] auto Date::getDay() const -> std::chrono::day {
    return ymd_.day();
}

[[nodiscard]] auto Date::getWeekday() const -> std::chrono::weekday {
    return std::chrono::weekday{ymd_};
}

[[nodiscard]] auto Date::isToday() const -> bool {
    return Date::today().getYMD() == ymd_;
}

[[nodiscard]] auto Date::toJSON() const & -> nlohmann::json {
    return {{"date", std::format("{:%Y-%m-%d}", ymd_)}};
}

[[nodiscard]] auto Date::fromJSON(const nlohmann::json &json) -> Date {
    if (!json.contains("date")) {
        throw std::runtime_error("Missing required fields");
    }

    std::string ymdString{json.at("date").get<std::string>()};

    constexpr int yearDigits{4};
    constexpr int monthDigits{2};
    constexpr int dayDigits{2};

    auto yearString{std::stoi(ymdString.substr(0, yearDigits))};

    auto monthString{static_cast<unsigned>(
        std::stoi(ymdString.substr(yearDigits + 1, monthDigits)))};

    auto dayString{static_cast<unsigned>(
        std::stoi(ymdString.substr(yearDigits + monthDigits + 1, dayDigits)))};

    std::chrono::year_month_day ymd{std::chrono::year{yearString},
                                    std::chrono::month{monthString},
                                    std::chrono::day{dayString}};

    return Date{ymd};
}

}; // namespace hbt::mods
