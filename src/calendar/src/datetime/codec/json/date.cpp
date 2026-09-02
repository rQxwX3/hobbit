#include <datetime/codec/json/date.hpp>

namespace clndr::dt::codec::json::date::processors::iso8601 {
[[nodiscard]] auto Encoder::operator()(const dt::Date &date) -> std::string {
    return std::format(format, date.getYear(), date.getMonth(), date.getDay());
}

[[nodiscard]] auto Decoder::operator()(const std::string &string) -> dt::Date {
    std::smatch matches;
    if (!std::regex_match(string, matches, regex)) {
        throw Error::RegexMismatch{};
    }

    constexpr size_t yearGroup{1};
    constexpr size_t monthGroup{2};
    constexpr size_t dayGroup{3};

    constexpr auto groups{
        std::array<size_t, dayGroup>{yearGroup, monthGroup, dayGroup}};

    if (std::ranges::any_of(groups, [&matches](auto group) -> bool {
            return !matches[group].matched;
        })) {
        throw Error::UnmatchedUnit{};
    }

    auto yearValue{std::stoi(matches[yearGroup].str())};
    auto monthValue{std::stoi(matches[monthGroup].str())};
    auto dayValue{std::stoi(matches[dayGroup].str())};

    try {
        return dt::Date(yearValue, monthValue, dayValue);
    } catch (...) {
        throw Error::ConstructionFailed{};
    }
}
}; // namespace clndr::dt::codec::json::date::processors::iso8601
