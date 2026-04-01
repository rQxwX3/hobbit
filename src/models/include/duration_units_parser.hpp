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
class DurationUnitsParser {
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
    inline static const auto fullRegexPattern{
        std::regex(R"(^([\W_]*\d+[\W_]*[a-zA-Z]+[\W_]*){1,6}$)")};

    inline static const auto pairRegexPattern{
        std::regex(R"((\d+)([A-Za-z]+))")};
    inline static const size_t pairRegexPatternValueGroup{1};
    inline static const size_t pairRegexPatternUnitGroup{2};

  private:
    [[nodiscard]] static auto
    getAllSubstrings(std::string strings,
                     const std::unordered_set<std::string> &exclude)
        -> std::unordered_set<std::string>;

  private:
    inline static const std::array<std::unordered_set<std::string>,
                                   DurationUnits::unit_t::COUNT_>
        possibleValues{
            getAllSubstrings("years", {"e", "a", "r", "s"}),
            getAllSubstrings("months", {"m", "o", "n", "t", "h", "s"}),
            getAllSubstrings("weeks", {"e", "k", "s"}),
            getAllSubstrings("days", {"a", "y", "s"}),
            getAllSubstrings("hours", {"o", "u", "r", "s"}),
            getAllSubstrings("minutes", {"i", "n", "u", "t", "e", "s"}),
        };

    inline static const std::array<std::string, DurationUnits::unit_t::COUNT_>
        preferredNaturalLanguageValues{"year", "month", "week",
                                       "day",  "hour",  "minute"};

  private:
    inline static const UnitBucket yearBucket{
        DurationUnits::unit_t::YEAR,
        possibleValues[DurationUnits::unit_t::YEAR],
        [](DurationUnits &durationUnits, DurationUnits::value_t value) -> void {
            durationUnits.addYears(value);
        }};

    inline static const UnitBucket monthBucket{
        DurationUnits::unit_t::MONTH,
        possibleValues[DurationUnits::unit_t::MONTH],
        [](DurationUnits &durationUnits, DurationUnits::value_t value) -> void {
            durationUnits.addMonths(value);
        }};

    inline static const UnitBucket weekBucket{
        DurationUnits::unit_t::WEEK,
        possibleValues[DurationUnits::unit_t::WEEK],
        [](DurationUnits &durationUnits, DurationUnits::value_t value) -> void {
            durationUnits.addWeeks(value);
        }};

    inline static const UnitBucket dayBucket{
        DurationUnits::unit_t::DAY, possibleValues[DurationUnits::unit_t::DAY],
        [](DurationUnits &durationUnits, DurationUnits::value_t value) -> void {
            durationUnits.addDays(value);
        }};

    inline static const UnitBucket hourBucket{
        DurationUnits::unit_t::HOUR,
        possibleValues[DurationUnits::unit_t::HOUR],
        [](DurationUnits &durationUnits, DurationUnits::value_t value) -> void {
            durationUnits.addHours(value);
        }};

    inline static const UnitBucket minuteBucket{
        DurationUnits::unit_t::MINUTE,
        possibleValues[DurationUnits::unit_t::MINUTE],
        [](DurationUnits &durationUnits, DurationUnits::value_t value) -> void {
            durationUnits.addMinutes(value);
        }};

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
    [[nodiscard]] static auto fromNaturalLanguage(const std::string &input)
        -> std::optional<DurationUnits>;

  public:
    [[nodiscard]] static auto
    formatUnitValuePairToNaturalLanguage(DurationUnits::unitValuePair_t pair)
        -> std::string;

  public:
    [[nodiscard]] auto static toNaturalLanguage(
        const DurationUnits &durationUnits) -> std::string;
};
} // namespace hbt::mods::util
