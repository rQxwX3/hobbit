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

[[nodiscard]] auto
DurationUnitsParser::getBucketOfUnit(const std::string &unitString)
    -> std::optional<std::reference_wrapper<const UnitBucket>> {
    const auto normalizedUnitString{toLower(unitString)};

    for (const auto &bucket : buckets) {
        if (bucket.contains(normalizedUnitString)) {
            return std::cref(bucket);
        }
    }

    return std::nullopt;
}

[[nodiscard]] auto DurationUnitsParser::parseUnit(
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
DurationUnitsParser::parseAllUnits(const std::string &filteredInput,
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

[[nodiscard]] auto
DurationUnitsParser::fromNaturalLanguage(const std::string &input)
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

[[nodiscard]] auto DurationUnitsParser::formatUnitValuePairToNaturalLanguage(
    DurationUnits::unitValuePair_t pair) -> std::string {
    auto unit{pair.first};
    auto value{pair.second};

    auto unitString{preferredNaturalLanguageValues[unit]};
    auto valueString{std::to_string(value)};

    if (value == 1) {
        return valueString + unitString;
    }

    return valueString + unitString + "s";
}

[[nodiscard]] auto
DurationUnitsParser::toNaturalLanguage(const DurationUnits &durationUnits)
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
} // namespace hbt::mods::util
