#include <codec/iso8601/datetime.hpp>

namespace clndr::codec::iso8601 {
[[nodiscard]] auto DateTime::encode(const dt::DateTime &datetime)
    -> std::string {
    return std::format("{:%Y-%m-%dT%H:%M}", datetime.getValue());
}

[[nodiscard]] auto DateTime::decode(const std::string &string)
    -> std::expected<dt::DateTime, Error::Code> {
    std::smatch matches;
    if (!std::regex_match(string, matches, regex)) {
        return std::unexpected(Error::Code::RegexMismatch);
    }

    constexpr size_t yearGroup{1};
    constexpr size_t monthGroup{2};
    constexpr size_t dayGroup{3};
    constexpr size_t hourGroup{4};
    constexpr size_t minuteGroup{5};

    constexpr size_t secondGroup{6}; // not supported, probably never will
                                     // be (currently discarding the value)

    constexpr auto groups{std::array<size_t, minuteGroup>{
        yearGroup, monthGroup, dayGroup, hourGroup, minuteGroup}};

    if (std::ranges::any_of(groups, [&matches](auto group) -> bool {
            return !matches[group].matched;
        })) {
        return std::unexpected(Error::Code::UnmatchedUnit);
    }

    auto yearValue{std::stoi(matches[yearGroup].str())};
    auto monthValue{std::stoi(matches[monthGroup].str())};
    auto dayValue{std::stoi(matches[dayGroup].str())};
    auto hourValue{std::stoi(matches[hourGroup].str())};
    auto minuteValue{std::stoi(matches[minuteGroup].str())};

    auto date{dt::Date(yearValue, monthValue, dayValue)};
    if (!date.ok()) {
        return std::unexpected(Error::Code::FailedToValidateDate);
    }

    auto time{dt::Time(hourValue, minuteValue)};
    if (!time.ok()) {
        return std::unexpected(Error::Code::FailedToValidateTime);
    }

    return dt::DateTime(date, time);
}
}; // namespace clndr::codec::iso8601
