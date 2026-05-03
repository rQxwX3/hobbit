#include <interval_parser.hpp>

#include <algorithm>
#include <ranges>
#include <regex>

namespace hbt::mods::util {
[[nodiscard]] auto toLower(std::string_view string) -> std::string {
    auto result{std::string{string}};
    std::ranges::transform(result, result.begin(),
                           [](unsigned char c) { return std::tolower(c); });

    return result;
}

[[nodiscard]] auto NaturalLanguageParser::getAllSubstrings(
    std::string string, const std::unordered_set<std::string> &exclude)
    -> std::unordered_set<std::string> {
    auto result{std::unordered_set<std::string>{}};
    auto substringCount{string.size()};

    for (size_t mask{1}; mask < (1ULL << substringCount); ++mask) {
        auto substring{std::string{}};

        for (size_t i{0}; i < substringCount; ++i) {
            if ((mask & (1ULL << i)) != 0U) {
                substring += string[i];
            }
        }

        if (!exclude.contains(substring)) {
            result.insert(substring);
        }
    }

    return result;
}

[[nodiscard]] auto
NaturalLanguageParser::getBucketOfUnit(const std::string &unitString)
    -> std::expected<UnitBucket, Error> {
    const auto normalizedUnitString{toLower(unitString)};

    for (const auto &bucket : buckets) {
        if (bucket.contains(normalizedUnitString)) {
            return std::cref(bucket);
        }
    }

    return std::unexpected(Error::UnitBucketNotFound);
}

auto NaturalLanguageParser::parseUnit(const std::string &unit,
                                      Interval::value_t value,
                                      Interval &interval,
                                      matchedBuckets_t &matchedBuckets)
    -> void {

    auto res{getBucketOfUnit(unit)};
    if (!res) {
        throw std::runtime_error(errorMessage(Error::UnitBucketNotFound));
    }

    auto bucket{res.value()};
    auto unitOfBucket{bucket.getUnit()};

    if (matchedBuckets.test(unitOfBucket)) {
        throw std::runtime_error(errorMessage(Error::ParsingMatchedUnit));
    }

    matchedBuckets.set(unitOfBucket);
    bucket.addUnit(interval, value);
}

auto NaturalLanguageParser::parseAllUnits(const std::string &filteredInput,
                                          Interval &interval) -> void {
    auto matchedBuckets{matchedBuckets_t{}};

    for (auto it(std::sregex_iterator{filteredInput.begin(),
                                      filteredInput.end(), pairRegexPattern});
         it != std::sregex_iterator(); ++it) {
        const auto &match{*it};

        auto value{std::stoi(match[pairRegexPatternValueGroup].str())};
        auto unit{match[pairRegexPatternUnitGroup].str()};

        if (!Interval::isValidValue(value)) {
            throw std::runtime_error(errorMessage(Error::InvalidUnitValue));
        }

        try {
            parseUnit(unit, value, interval, matchedBuckets);
        } catch (std::runtime_error) {
            throw std::runtime_error(errorMessage(Error::FailedToParseUnit));
        }
    }
}

[[nodiscard]] auto filterInput(const std::string &input) -> std::string {
    std::string filtered;
    std::ranges::copy(input | std::ranges::views::filter([](unsigned char c) {
                          return std::isalnum(c);
                      }),
                      std::back_inserter(filtered));

    return filtered;
}

[[nodiscard]] auto NaturalLanguageParser::parse(const std::string &input)
    -> std::expected<Interval, Error> {
    if (!std::regex_match(input, fullRegexPattern)) {
        return std::unexpected(Error::RegexMismatch);
    }

    auto interval{Interval{}};
    auto filteredInput{filterInput(input)};

    try {
        parseAllUnits(filteredInput, interval);
        return interval;
    } catch (std::runtime_error) {
        return std::unexpected(Error::FailedToParseUnit);
    }
}

[[nodiscard]] auto NaturalLanguageParser::formatUnitValuePairToNaturalLanguage(
    Interval::unitValuePair_t pair) -> std::string {
    auto unit{pair.first};
    auto value{pair.second};

    auto unitString{preferredNaturalLanguageValues[unit]};
    auto valueString{std::to_string(value)};

    if (value == 1) {
        unitString.pop_back();
    }

    return valueString + unitString;
}

[[nodiscard]] auto NaturalLanguageParser::format(const Interval &interval)
    -> std::string {
    auto result{std::string{}};
    auto unitValuePairs{interval.getNonZeroUnitValuePairs()};

    for (const auto pair : unitValuePairs) {
        result += formatUnitValuePairToNaturalLanguage(pair) + ", ";
    }

    if (result.size() > 1) {
        result.pop_back(); // remove trailing comma
        result.pop_back(); // remove trailing space
    }

    return result;
}

[[nodiscard]] auto ISO8601IntervalParser::parse(const std::string &input)
    -> std::expected<Interval, Error> {
    std::smatch matches;
    if (!std::regex_match(input, matches, pattern)) {
        return std::unexpected(Error::RegexMismatch);
    }

    Interval result{};

    auto parseGroup{[&matches, &result](unit_t unit) -> void {
        auto unitGroup{patternUnitGroups[unit]};

        if (matches[unitGroup].matched) {
            auto groupString{matches[unitGroup].str()};

            groupString.pop_back(); // remove trailing 'Y', 'M', etc.
            result.addUnit(
                unit, static_cast<Interval::value_t>(std::stoll(groupString)));
        }
    }};

    for (auto unit{static_cast<size_t>(unit_t::YEAR)}; unit != unit_t::COUNT_;
         ++unit) {
        parseGroup(static_cast<unit_t>(unit));
    }

    return result;
}

[[nodiscard]] auto ISO8601IntervalParser::format(const Interval &interval)
    -> std::string {
    if (interval.isZero()) {
        return zeroValueFormat;
    }

    auto result{std::string{formatPrefix}};
    auto timeSectionStarted{false};

    auto formatDateGroup{[&interval, &result](unit_t unit) -> void {
        if (const auto value{interval.getUnitValue(unit)}; value) {
            result += std::to_string(value);
            result += unitSeparators[unit];
        }
    }};

    auto formatTimeGroup{
        [&timeSectionStarted, &interval, &result](unit_t unit) -> void {
            if (const auto value{interval.getUnitValue(unit)}; value) {
                if (!timeSectionStarted) {
                    result += timeSectionSeparator;
                    timeSectionStarted = true;
                }

                result += std::to_string(value);
                result += unitSeparators[unit];
            }
        }};

    for (auto unit{static_cast<size_t>(unit_t::YEAR)}; unit != unit_t::HOUR;
         ++unit) {
        formatDateGroup(static_cast<unit_t>(unit));
    }

    for (auto unit{static_cast<size_t>(unit_t::HOUR)}; unit != unit_t::COUNT_;
         ++unit) {
        formatTimeGroup(static_cast<unit_t>(unit));
    }

    return result;
}
} // namespace hbt::mods::util
