#include <datetime/codec/json/time.hpp>

namespace clndr::dt::codec::json::time::processors::iso8601 {
[[nodiscard]] auto Encoder::operator()(const dt::Time &time) -> std::string {
    return std::format(format, time.getHour(), time.getMinute());
}

[[nodiscard]] auto Decoder::operator()(const std::string &string) -> dt::Time {
    std::smatch matches;
    if (!std::regex_match(string, matches, regex)) {
        throw Error::RegexMismatch{};
    }

    constexpr size_t hourGroup{1};
    constexpr size_t minuteGroup{2};

    constexpr auto groups{
        std::array<size_t, minuteGroup>{hourGroup, minuteGroup}};

    if (std::ranges::any_of(groups, [&matches](auto group) -> bool {
            return !matches[group].matched;
        })) {
        throw Error::UnmatchedUnit{};
    }

    auto hourValue{std::stoi(matches[hourGroup].str())};
    auto minuteValue{std::stoi(matches[minuteGroup].str())};

    try {
        return dt::Time(hourValue, minuteValue);
    } catch (...) {
        throw Error::ConstructionFailed{};
    }
}
}; // namespace clndr::dt::codec::json::time::processors::iso8601
