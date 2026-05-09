#pragma once

#include <recurrence_pattern.hpp>

namespace hbt::mods::util {
class IntervalRecurrencePattern : public RecurrencePattern {
  private:
    enum class Error : uint8_t {
        JSONFailedToParseInterval,

        InvalidInterval,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONFailedToParseInterval:
            return "IntervalRecurrencePattern: failed to parse Interval from "
                   "JSON";

        case Error::InvalidInterval:
            return "IntervalRecurrencePattern: cannot instantiate recurrence "
                   "pattern from zero Interval";

        default:
            return "IntervalRecurrencePattern: unclassified error";
        }
    }

  private:
    static auto validateInterval(const Interval &interval) -> Interval;

  private:
    Interval interval_;

  public:
    IntervalRecurrencePattern(const Interval &interval);

  public:
    [[nodiscard]] auto getInterval() const -> Interval;

  public:
    [[nodiscard]] auto happensOnDate(DateTime start, DateTime on) const -> bool;

  private:
    [[nodiscard]] auto getFirstOccurrenceOfDate(DateTime start,
                                                DateTime on) const
        -> std::optional<occurrence_t>;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime start, DateTime on) const
        -> occurrences_t override;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::expected<IntervalRecurrencePattern, Error>;
};
} // namespace hbt::mods::util
