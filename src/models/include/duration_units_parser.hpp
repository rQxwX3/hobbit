#pragma once

#include <array>
#include <bitset>
#include <functional>
#include <optional>
#include <regex>
#include <string>
#include <unordered_set>

#include <duration_units.hpp>

namespace hbt::mods::util {
template <typename T>
concept DurationUnitsParserConcept =
    requires(T parser, const std::string &s, const DurationUnits &u) {
        { parser.parse(s) } -> std::same_as<std::optional<DurationUnits>>;
        { parser.format(u) } -> std::convertible_to<std::string>;
    };

class NaturalLanguageParser {
  private:
    class UnitBucket {
      private:
        using possibleValues_t = std::unordered_set<std::string>;
        using addUnitCallback_t = std::function<void(
            DurationUnits &durationUnits, DurationUnits::value_t value)>;

      private:
        DurationUnits::unit_t unit;
        possibleValues_t possibleValues_;
        addUnitCallback_t addUnitCallback_;

      public:
        UnitBucket(DurationUnits::unit_t unit, possibleValues_t possibleValues,
                   addUnitCallback_t addUnitCallback)
            : unit{unit}, possibleValues_{std::move(possibleValues)},
              addUnitCallback_{std::move(addUnitCallback)} {}

      public:
        [[nodiscard]] constexpr auto getUnit() const -> DurationUnits::unit_t {
            return unit;
        }

        [[nodiscard]] auto contains(const std::string &unitString) const
            -> bool {
            return possibleValues_.contains(unitString);
        }

        auto addUnit(DurationUnits &durationUnits, DurationUnits::value_t value)
            -> bool {
            addUnitCallback_(durationUnits, value);
            return true;
        }
    };

    using unitBuckets_t = std::array<UnitBucket, DurationUnits::unit_t::COUNT_>;
    using matchedBuckets_t = std::bitset<DurationUnits::unit_t::COUNT_>;

  private:
    [[nodiscard]] static auto
    getAllSubstrings(std::string strings,
                     const std::unordered_set<std::string> &exclude)
        -> std::unordered_set<std::string>;

  private:
    inline static const auto fullRegexPattern{
        std::regex(R"(^([\W_]*\d+[\W_]*[a-zA-Z]+[\W_]*){1,6}$)")};

    inline static const auto pairRegexPattern{
        std::regex(R"((\d+)([A-Za-z]+))")};
    inline static const size_t pairRegexPatternValueGroup{1};
    inline static const size_t pairRegexPatternUnitGroup{2};

  private:
    inline static const std::array<std::string, DurationUnits::unit_t::COUNT_>
        preferredNaturalLanguageValues{"years", "months", "weeks",
                                       "days",  "hours",  "minutes"};

    inline static const std::array<std::unordered_set<std::string>,
                                   DurationUnits::unit_t::COUNT_>
        possibleValues{
            getAllSubstrings(
                preferredNaturalLanguageValues[DurationUnits::unit_t::YEAR],
                {"e", "a", "r", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[DurationUnits::unit_t::MONTH],
                {"o", "n", "t", "h", "s", "hs"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[DurationUnits::unit_t::WEEK],
                {"e", "k", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[DurationUnits::unit_t::DAY],
                {"a", "y", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[DurationUnits::unit_t::HOUR],
                {"o", "u", "r", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[DurationUnits::unit_t::MINUTE],
                {"m", "i", "n", "u", "t", "e", "s", "ms", "mt"}),
        };

  private:
    static auto createUnitBucket(DurationUnits::unit_t unit) -> UnitBucket {
        return UnitBucket{unit, possibleValues[unit],
                          [unit](DurationUnits &durationUnits,
                                 DurationUnits::value_t value) -> void {
                              durationUnits.addUnit(unit, value);
                          }};
    }

  private:
    inline static const auto yearBucket{
        createUnitBucket(DurationUnits::unit_t::YEAR)};

    inline static const auto monthBucket{
        createUnitBucket(DurationUnits::unit_t::MONTH)};

    inline static const auto weekBucket{
        createUnitBucket(DurationUnits::unit_t::WEEK)};

    inline static const auto dayBucket{
        createUnitBucket(DurationUnits::unit_t::DAY)};

    inline static const auto hourBucket{
        createUnitBucket(DurationUnits::unit_t::HOUR)};

    inline static const auto minuteBucket{
        createUnitBucket(DurationUnits::unit_t::MINUTE)};

    inline static unitBuckets_t buckets{yearBucket, monthBucket, weekBucket,
                                        dayBucket,  hourBucket,  minuteBucket};

  private:
    [[nodiscard]] static auto getBucketOfUnit(const std::string &unitString)
        -> std::optional<std::reference_wrapper<const UnitBucket>>;

    [[nodiscard]] static auto
    parseUnit(const std::string &unit, DurationUnits::value_t value,
              DurationUnits &durationUnits, matchedBuckets_t &matchedBuckets)
        -> bool;

    [[nodiscard]] static auto parseAllUnits(const std::string &filteredInput,
                                            DurationUnits &durationUnits)
        -> bool;

  public:
    [[nodiscard]] static auto parse(const std::string &input)
        -> std::optional<DurationUnits>;

  public:
    [[nodiscard]] static auto
    formatUnitValuePairToNaturalLanguage(DurationUnits::unitValuePair_t pair)
        -> std::string;

  public:
    [[nodiscard]] auto static format(const DurationUnits &durationUnits)
        -> std::string;
};

class ISO8601DurationParser {
  private:
    using unit_t = DurationUnits::unit_t;

  private:
    /*
     * ISO8601 Duration regex pattern adapted from:
     * https://stackoverflow.com/a/32045167
     * (modified to exclude seconds group)
     */
    static inline const auto pattern{std::regex{
        R"(^P(?!$)(\d+(?:\.\d+)?Y)?(\d+(?:\.\d+)?M)?(\d+(?:\.\d+)?W)?(\d+(?:\.\d+)?D)?(?:T(?=[\d])(?:(\d+(?:\.\d+)?H)?(\d+(?:\.\d+)?M)?)?)?$)"}};

    static constexpr std::array<size_t, unit_t::COUNT_> patternUnitGroups{
        1, 2, 3, 4, 5, 6};

  private:
    static constexpr auto zeroValueFormat{"PT0M"};
    static constexpr auto formatPrefix{'P'};
    static constexpr auto timeSectionSeparator{'T'};

    static constexpr std::array<char, unit_t::COUNT_> unitSeparators{
        'Y', 'M', 'W', 'D', 'H', 'M'};

  public:
    [[nodiscard]] static auto parse(const std::string &input)
        -> std::optional<DurationUnits>;

    [[nodiscard]] auto static format(const DurationUnits &durationUnits)
        -> std::string;
};

template <typename Parser>
    requires DurationUnitsParserConcept<Parser>
class DurationUnitsParser {
  public:
    [[nodiscard]] static auto parse(const std::string &input)
        -> std::optional<DurationUnits> {
        return Parser::parse(input);
    }

    [[nodiscard]] static auto format(const DurationUnits &durationUnits)
        -> std::string {
        return Parser::format(durationUnits);
    }
};
} // namespace hbt::mods::util
