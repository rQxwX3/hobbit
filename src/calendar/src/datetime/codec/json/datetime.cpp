#include <datetime/codec/json/date.hpp>
#include <datetime/codec/json/datetime.hpp>
#include <datetime/codec/json/time.hpp>

namespace clndr::dt::codec::json::datetime::processors::iso8601 {
[[nodiscard]] auto Encoder::operator()(const dt::DateTime &datetime)
    -> std::string {
    return std::format("{:%Y-%m-%dT%H:%M}", datetime.getValue());
}

[[nodiscard]] auto Decoder::operator()(const std::string &string)
    -> dt::DateTime {
    std::smatch matches;
    if (!std::regex_match(string, matches, regex)) {
        throw Error::RegexMismatch{};
    }

    constexpr size_t dateGroup{1};
    constexpr size_t timeGroup{2};

    constexpr auto groups{std::array<size_t, timeGroup>{dateGroup, timeGroup}};

    if (std::ranges::any_of(groups, [&matches](auto group) -> bool {
            return !matches[group].matched;
        })) {
        throw Error::UnmatchedDateOrTime{};
    }

    auto dateValue{codec::json::date::Codec::decode(matches[dateGroup].str())};
    auto timeValue{codec::json::time::Codec::decode(matches[timeGroup].str())};

    try {
        return dt::DateTime(dateValue, timeValue);
    } catch (...) {
        throw Error::ConstructionFailed{};
    }
}
}; // namespace clndr::dt::codec::json::datetime::processors::iso8601
