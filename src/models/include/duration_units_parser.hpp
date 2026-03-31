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
        using addUnitCallback_t =
            std::function<void(DurationUnits &durationUnits, int value)>;

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

        auto addUnit(DurationUnits &durationUnits, int value) -> bool {
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
    inline static const UnitBucket yearBucket{
        DurationUnits::unit_t::YEAR,
        {"y", "yr", "yrs", "year", "years"},
        [](DurationUnits &durationUnits, int value) -> void {
            durationUnits.addYears(value);
        }};

    inline static const UnitBucket monthBucket{
        DurationUnits::unit_t::MONTH,
        {"mo", "month", "months"},
        [](DurationUnits &durationUnits, int value) -> void {
            durationUnits.addMonths(value);
        }};

    inline static const UnitBucket weekBucket{
        DurationUnits::unit_t::WEEK,
        {"w", "wk", "wks", "week", "weeks"},
        [](DurationUnits &durationUnits, int value) -> void {
            durationUnits.addWeeks(value);
        }};

    inline static const UnitBucket dayBucket{
        DurationUnits::unit_t::DAY,
        {"d", "day", "days"},
        [](DurationUnits &durationUnits, int value) -> void {
            durationUnits.addDays(value);
        }};

    inline static const UnitBucket hourBucket{
        DurationUnits::unit_t::HOUR,
        {"h", "hr", "hrs", "hour", "hours"},
        [](DurationUnits &durationUnits, int value) -> void {
            durationUnits.addHours(value);
        }};

    inline static const UnitBucket minuteBucket{
        DurationUnits::unit_t::MINUTE,
        {"m", "min", "mins", "minute", "minutes"},
        [](DurationUnits &durationUnits, int value) -> void {
            durationUnits.addMinutes(value);
        }};

    inline static unitBuckets_t buckets{yearBucket, monthBucket, weekBucket,
                                        dayBucket,  hourBucket,  minuteBucket};

  private:
    [[nodiscard]] static auto getBucketOfUnit(const std::string &unitString)
        -> std::optional<std::reference_wrapper<const UnitBucket>>;

    [[nodiscard]] static auto parseUnit(const std::string &unit, int value,
                                        DurationUnits &durationUnits,
                                        matchedBuckets_t &matchedBuckets)
        -> bool;

    [[nodiscard]] static auto parseAllUnits(const std::string &filteredInput,
                                            DurationUnits &durationUnits)
        -> bool;

  public:
    [[nodiscard]] static auto fromNaturalLanguage(const std::string &input)
        -> std::optional<DurationUnits>;
};
} // namespace hbt::mods::util
