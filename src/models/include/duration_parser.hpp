#pragma once

#include <array>
#include <bitset>
#include <expected>
#include <functional>
#include <regex>
#include <string>
#include <unordered_set>

#include <duration.hpp>

namespace hbt::mods::util {
template <typename T>
concept DurationParserConcept =
    requires(T parser, const std::string &s, const Duration &u) {
        {
            parser.parse(s)
        } -> std::same_as<std::expected<Duration, typename T::Error>>;

        { parser.format(u) } -> std::convertible_to<std::string>;
    };

class NaturalLanguageParser {
  public:
    enum class Error : uint8_t {
        RegexMismatch,
        UnitBucketNotFound,
        ParsingMatchedUnit,
        InvalidUnitValue,
        FailedToParseUnit,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::RegexMismatch:
            return "NaturalLanguageParser: provided input didn't match regex";

        case Error::UnitBucketNotFound:
            return "NaturalLanguageParser: unit bucket(s) not initialized";

        case Error::ParsingMatchedUnit:
            return "NaturalLanguageParser: provided input contains the same "
                   "unit more than once";

        case Error::InvalidUnitValue:
            return "NaturalLanguageParser: provided input contains invalid "
                   "value (possible overflow)";

        case Error::FailedToParseUnit:
            return "NaturalLanguageParser: couldn't parse one or more units";
        }
    }

  private:
    class UnitBucket {
      private:
        using possibleValues_t = std::unordered_set<std::string>;
        using addUnitCallback_t =
            std::function<void(Duration &duration, Duration::value_t value)>;

      private:
        Duration::unit_t unit;
        possibleValues_t possibleValues_;
        addUnitCallback_t addUnitCallback_;

      public:
        UnitBucket(Duration::unit_t unit, possibleValues_t possibleValues,
                   addUnitCallback_t addUnitCallback)
            : unit{unit}, possibleValues_{std::move(possibleValues)},
              addUnitCallback_{std::move(addUnitCallback)} {}

      public:
        [[nodiscard]] constexpr auto getUnit() const -> Duration::unit_t {
            return unit;
        }

        [[nodiscard]] auto contains(const std::string &unitString) const
            -> bool {
            return possibleValues_.contains(unitString);
        }

        auto addUnit(Duration &duration, Duration::value_t value) -> void {
            addUnitCallback_(duration, value);
        }
    };

    using unitBuckets_t = std::array<UnitBucket, Duration::unit_t::COUNT_>;
    using matchedBuckets_t = std::bitset<Duration::unit_t::COUNT_>;

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
    inline static const std::array<std::string, Duration::unit_t::COUNT_>
        preferredNaturalLanguageValues{"years", "months", "weeks",
                                       "days",  "hours",  "minutes"};

    inline static const std::array<std::unordered_set<std::string>,
                                   Duration::unit_t::COUNT_>
        possibleValues{
            getAllSubstrings(
                preferredNaturalLanguageValues[Duration::unit_t::YEAR],
                {"e", "a", "r", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Duration::unit_t::MONTH],
                {"o", "n", "t", "h", "s", "hs"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Duration::unit_t::WEEK],
                {"e", "k", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Duration::unit_t::DAY],
                {"a", "y", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Duration::unit_t::HOUR],
                {"o", "u", "r", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Duration::unit_t::MINUTE],
                {"m", "i", "n", "u", "t", "e", "s", "ms", "mt"}),
        };

  private:
    static auto createUnitBucket(Duration::unit_t unit) -> UnitBucket {
        return UnitBucket{
            unit, possibleValues[unit],
            [unit](Duration &duration, Duration::value_t value) -> void {
                duration.addUnit(unit, value);
            }};
    }

  private:
    inline static const auto yearBucket{
        createUnitBucket(Duration::unit_t::YEAR)};

    inline static const auto monthBucket{
        createUnitBucket(Duration::unit_t::MONTH)};

    inline static const auto weekBucket{
        createUnitBucket(Duration::unit_t::WEEK)};

    inline static const auto dayBucket{createUnitBucket(Duration::unit_t::DAY)};

    inline static const auto hourBucket{
        createUnitBucket(Duration::unit_t::HOUR)};

    inline static const auto minuteBucket{
        createUnitBucket(Duration::unit_t::MINUTE)};

    inline static unitBuckets_t buckets{yearBucket, monthBucket, weekBucket,
                                        dayBucket,  hourBucket,  minuteBucket};

  private:
    [[nodiscard]] static auto getBucketOfUnit(const std::string &unitString)
        -> std::expected<UnitBucket, Error>;

    static auto parseUnit(const std::string &unit, Duration::value_t value,
                          Duration &duration, matchedBuckets_t &matchedBuckets)
        -> void;

    static auto parseAllUnits(const std::string &filteredInput,
                              Duration &duration) -> void;

  private:
    [[nodiscard]] static auto
    formatUnitValuePairToNaturalLanguage(Duration::unitValuePair_t pair)
        -> std::string;

  public:
    [[nodiscard]] auto static format(const Duration &duration) -> std::string;

    [[nodiscard]] static auto parse(const std::string &input)
        -> std::expected<Duration, Error>;
};

class ISO8601DurationParser {
  public:
    enum class Error : uint8_t { RegexMismatch };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::RegexMismatch:
            return "ISO8601DurationParser: provided input didn't match regex";
        }
    }

  private:
    using unit_t = Duration::unit_t;

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
        -> std::expected<Duration, Error>;

    [[nodiscard]] auto static format(const Duration &duration) -> std::string;
};

template <typename Parser>
    requires DurationParserConcept<Parser>
class DurationParser {
  public:
    [[nodiscard]] static auto parse(const std::string &input)
        -> std::expected<Duration, typename Parser::Error> {
        return Parser::parse(input);
    }

    [[nodiscard]] static auto format(const Duration &duration) -> std::string {
        return Parser::format(duration);
    }
};
} // namespace hbt::mods::util
