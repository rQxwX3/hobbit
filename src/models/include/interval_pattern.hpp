#pragma once

#include <recurrence_pattern.hpp>

namespace hbt::mods::util {
class IntervalRecurrencePattern : public RecurrencePattern {
  public:
    enum class Error : uint8_t {
        InvalidInterval,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidInterval:
            return "IntervalRecurrencePattern: cannot instantiate from zero "
                   "Interval";

        default:
            std::unreachable();
        }
    }

  private:
    static auto validateInterval(const Interval &interval) -> Interval;

  private:
    Interval interval_;

  public:
    IntervalRecurrencePattern(Interval interval);

  public:
    [[nodiscard]] auto getInterval() const -> Interval;

  public:
    [[nodiscard]] auto happensOnDate(DateTime date, DateTime start) const
        -> bool;

  public:
    [[nodiscard]] auto getFirstOccurrenceOfDate(DateTime date,
                                                DateTime start) const
        -> std::optional<occurrence_t>;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime date, DateTime start) const
        -> occurrences_t override;

  public:
    [[nodiscard]] auto operator==(const IntervalRecurrencePattern &other) const
        -> bool;

  public:
    struct JSON {
        enum class Error : uint8_t {
            MissingRequiredField,

            FailedToParseInterval,
            ParsedIntervalZero,
        };

        [[nodiscard]] static constexpr auto errorMessage(Error error)
            -> std::string {
            switch (error) {
            case Error::MissingRequiredField:
                return "IntervalRecurrencePattern::JSON: missing required "
                       "field(s)";

            case Error::FailedToParseInterval:
                return "IntervalRecurrencePattern::JSON: failed to parse "
                       "Interval";

            case Error::ParsedIntervalZero:
                return "IntervalRecurrencePattern::JSON: parsed Interval is a "
                       "zero-interval";

            default:
                std::unreachable();
            }
        }

        static constexpr auto intervalField{std::string_view{"interval"}};

        static constexpr auto fields{
            std::array<std::string_view, 1>{intervalField}};

        [[nodiscard]] static auto containsAllFields(const nlohmann::json &json)
            -> bool;

        [[nodiscard]] static auto
        encode(const IntervalRecurrencePattern &pattern) -> nlohmann::json;

        [[nodiscard]] auto static decode(const nlohmann::json &json)
            -> std::expected<IntervalRecurrencePattern, JSON::Error>;
    };
};
} // namespace hbt::mods::util
