#pragma once

#include <array>
#include <bitset>
#include <expected>
#include <functional>
#include <regex>
#include <string>
#include <unordered_set>

#include <interval.hpp>

namespace hbt::mods::util {
template <typename T>
concept IntervalParserConcept =
    requires(T parser, const std::string &s, const Interval &u) {
        {
            parser.parse(s)
        } -> std::same_as<std::expected<Interval, typename T::Error>>;

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

        default:
            return "NaturalLanguageParser: unclassified error";
        }
    }

  private:
    class UnitBucket {
      private:
        using possibleValues_t = std::unordered_set<std::string>;
        using addUnitCallback_t =
            std::function<void(Interval &interval, Interval::value_t value)>;

      private:
        Interval::Unit unit;
        possibleValues_t possibleValues_;
        addUnitCallback_t addUnitCallback_;

      public:
        UnitBucket(Interval::Unit unit, possibleValues_t possibleValues,
                   addUnitCallback_t addUnitCallback)
            : unit{unit}, possibleValues_{std::move(possibleValues)},
              addUnitCallback_{std::move(addUnitCallback)} {}

      public:
        [[nodiscard]] constexpr auto getUnit() const -> Interval::Unit {
            return unit;
        }

        [[nodiscard]] auto contains(const std::string &unitString) const
            -> bool {
            return possibleValues_.contains(unitString);
        }

        auto addUnit(Interval &interval, Interval::value_t value) -> void {
            addUnitCallback_(interval, value);
        }
    };

    using unitBuckets_t = std::array<UnitBucket, Interval::Unit::COUNT_>;
    using matchedBuckets_t = std::bitset<Interval::Unit::COUNT_>;

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
    inline static const std::array<std::string, Interval::Unit::COUNT_>
        preferredNaturalLanguageValues{"years", "months", "weeks",
                                       "days",  "hours",  "minutes"};

    inline static const std::array<std::unordered_set<std::string>,
                                   Interval::Unit::COUNT_>
        possibleValues{
            getAllSubstrings(
                preferredNaturalLanguageValues[Interval::Unit::YEAR],
                {"e", "a", "r", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Interval::Unit::MONTH],
                {"o", "n", "t", "h", "s", "hs"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Interval::Unit::WEEK],
                {"e", "k", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Interval::Unit::DAY],
                {"a", "y", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Interval::Unit::HOUR],
                {"o", "u", "r", "s"}),

            getAllSubstrings(
                preferredNaturalLanguageValues[Interval::Unit::MINUTE],
                {"m", "i", "n", "u", "t", "e", "s", "ms", "mt"}),
        };

  private:
    static auto createUnitBucket(Interval::Unit unit) -> UnitBucket {
        return UnitBucket{
            unit, possibleValues[unit],
            [unit](Interval &interval, Interval::value_t value) -> void {
                interval.addUnit(unit, value);
            }};
    }

  private:
    inline static const auto yearBucket{createUnitBucket(Interval::Unit::YEAR)};

    inline static const auto monthBucket{
        createUnitBucket(Interval::Unit::MONTH)};

    inline static const auto weekBucket{createUnitBucket(Interval::Unit::WEEK)};

    inline static const auto dayBucket{createUnitBucket(Interval::Unit::DAY)};

    inline static const auto hourBucket{createUnitBucket(Interval::Unit::HOUR)};

    inline static const auto minuteBucket{
        createUnitBucket(Interval::Unit::MINUTE)};

    inline static unitBuckets_t buckets{yearBucket, monthBucket, weekBucket,
                                        dayBucket,  hourBucket,  minuteBucket};

  private:
    [[nodiscard]] static auto getBucketOfUnit(const std::string &unitString)
        -> std::expected<UnitBucket, Error>;

    static auto parseUnit(const std::string &unit, Interval::value_t value,
                          Interval &interval, matchedBuckets_t &matchedBuckets)
        -> void;

    static auto parseAllUnits(const std::string &filteredInput,
                              Interval &interval) -> void;

  private:
    [[nodiscard]] static auto
    formatUnitValuePairToNaturalLanguage(Interval::unitValuePair_t pair)
        -> std::string;

  public:
    [[nodiscard]] auto static format(const Interval &interval) -> std::string;

    [[nodiscard]] static auto parse(const std::string &input)
        -> std::expected<Interval, Error>;
};

class ISO8601IntervalParser {
  public:
    enum class Error : uint8_t { RegexMismatch, InvalidUnitValue };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::RegexMismatch:
            return "ISO8601IntervalParser: provided input didn't match regex";

        case Error::InvalidUnitValue:
            return "ISO8601IntervalParser: provided input contains invalid "
                   "value (possible overflow)";

        default:
            std::unreachable();
        }
    }

  private:
    using Unit = Interval::Unit;

  private:
    /*
     * ISO8601 duration regex pattern adapted from:
     * https://stackoverflow.com/a/32045167
     * (modified to exclude seconds group)
     */
    static inline const auto pattern{std::regex{
        R"(^P(?!$)(\d+(?:\.\d+)?Y)?(\d+(?:\.\d+)?M)?(\d+(?:\.\d+)?W)?(\d+(?:\.\d+)?D)?(?:T(?=[\d])(?:(\d+(?:\.\d+)?H)?(\d+(?:\.\d+)?M)?)?)?$)"}};

    static constexpr std::array<size_t, Unit::COUNT_> patternUnitGroups{
        1, 2, 3, 4, 5, 6};

  private:
    static constexpr auto zeroValueFormat{"PT0M"};
    static constexpr auto formatPrefix{'P'};
    static constexpr auto timeSectionSeparator{'T'};

    static constexpr std::array<char, Unit::COUNT_> unitSeparators{
        'Y', 'M', 'W', 'D', 'H', 'M'};

  public:
    [[nodiscard]] static auto parse(const std::string &input)
        -> std::expected<Interval, Error>;

    [[nodiscard]] auto static format(const Interval &interval) -> std::string;
};

template <typename Parser>
    requires IntervalParserConcept<Parser>
class IntervalParser {
  public:
    [[nodiscard]] static auto parse(const std::string &input)
        -> std::expected<Interval, typename Parser::Error> {
        return Parser::parse(input);
    }

    [[nodiscard]] static auto format(const Interval &interval) -> std::string {
        return Parser::format(interval);
    }
};
} // namespace hbt::mods::util
