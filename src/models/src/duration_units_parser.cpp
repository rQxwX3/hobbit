#include <duration_units_parser.hpp>

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
    -> std::optional<std::reference_wrapper<const UnitBucket>> {
    const auto normalizedUnitString{toLower(unitString)};

    for (const auto &bucket : buckets) {
        if (bucket.contains(normalizedUnitString)) {
            return std::cref(bucket);
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto NaturalLanguageParser::parseUnit(
    const std::string &unit, DurationUnits::value_t value,
    DurationUnits &durationUnits, matchedBuckets_t &matchedBuckets) -> bool {

    auto res{getBucketOfUnit(unit)};
    if (!res.has_value()) {
        return false;
    }

    auto bucket{res.value().get()};
    auto bucketUnit{bucket.getUnit()};

    if (matchedBuckets.test(bucketUnit)) {
        return false;
    }

    matchedBuckets.set(bucketUnit);
    return bucket.addUnit(durationUnits, value);
}

[[nodiscard]] auto
NaturalLanguageParser::parseAllUnits(const std::string &filteredInput,
                                     DurationUnits &durationUnits) -> bool {
    auto matchedBuckets{matchedBuckets_t{}};

    for (auto it(std::sregex_iterator{filteredInput.begin(),
                                      filteredInput.end(), pairRegexPattern});
         it != std::sregex_iterator(); ++it) {
        const auto &match{*it};

        auto value{std::stoi(match[pairRegexPatternValueGroup].str())};
        auto unit{match[pairRegexPatternUnitGroup].str()};

        if (!DurationUnits::isValidValue(value)) {
            return false;
        }

        if (!parseUnit(unit, value, durationUnits, matchedBuckets)) {
            return false;
        }
    }

    return true;
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
    -> std::optional<DurationUnits> {
    if (!std::regex_match(input, fullRegexPattern)) {
        return std::nullopt;
    }

    auto durationUnits{DurationUnits{}};
    auto filteredInput{filterInput(input)};

    if (parseAllUnits(filteredInput, durationUnits)) {
        return durationUnits;
    }

    return std::nullopt;
}

[[nodiscard]] auto NaturalLanguageParser::formatUnitValuePairToNaturalLanguage(
    DurationUnits::unitValuePair_t pair) -> std::string {
    auto unit{pair.first};
    auto value{pair.second};

    auto unitString{preferredNaturalLanguageValues[unit]};
    auto valueString{std::to_string(value)};

    if (value == 1) {
        unitString.pop_back();
    }

    return valueString + unitString;
}

[[nodiscard]] auto
NaturalLanguageParser::format(const DurationUnits &durationUnits)
    -> std::string {
    auto result{std::string{}};
    auto unitValuePairs{durationUnits.getNonZeroUnitValuePairs()};

    for (const auto pair : unitValuePairs) {
        result += formatUnitValuePairToNaturalLanguage(pair) + ", ";
    }

    if (result.size() > 1) {
        result.pop_back(); // remove trailing comma
        result.pop_back(); // remove trailing space
    }

    return result;
}

[[nodiscard]] auto ISO8601DurationParser::parse(const std::string &input)
    -> std::optional<DurationUnits> {
    std::smatch matches;
    if (!std::regex_match(input, matches, pattern)) {
        return std::nullopt;
    }

    DurationUnits result;

    auto parseGroup{[&matches, &result](unit_t unit) -> void {
        auto unitGroup{patternUnitGroups[unit]};

        if (matches[unitGroup].matched) {
            auto groupString{matches[unitGroup].str()};

            groupString.pop_back(); // remove trailing 'Y', 'M', etc.
            result.addUnit(unit, static_cast<DurationUnits::value_t>(
                                     std::stoll(groupString)));
        }
    }};

    for (auto unit{static_cast<size_t>(unit_t::YEAR)}; unit != unit_t::COUNT_;
         ++unit) {
        parseGroup(static_cast<unit_t>(unit));
    }

    return result;
}

[[nodiscard]] auto
ISO8601DurationParser::format(const DurationUnits &durationUnits)
    -> std::string {
    if (durationUnits.isZero()) {
        return zeroValueFormat;
    }

    auto result{std::string{formatPrefix}};
    auto timeSectionStarted{false};

    auto formatDateGroup{[&durationUnits, &result](unit_t unit) -> void {
        if (const auto value{durationUnits.getUnitValue(unit)}; value) {
            result += std::to_string(value);
            result += unitSeparators[unit];
        }
    }};

    auto formatTimeGroup{
        [&timeSectionStarted, &durationUnits, &result](unit_t unit) -> void {
            if (const auto value{durationUnits.getUnitValue(unit)}; value) {
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
