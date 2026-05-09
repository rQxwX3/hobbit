#pragma once

#include <recurrence_pattern.hpp>

namespace hbt::mods::util {
class IntervalRecurrencePattern : public RecurrencePattern {
  private:
    enum class Error : uint8_t {
        JSONMissingRequiredField,
        JSONFailedToParseStart,
        JSONFailedToParseInterval,

        InvalidInterval,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "IntervalRecurrencePattern: missing required field(s) in "
                   "JSON";

        case Error::JSONFailedToParseStart:
            return "IntervalRecurrencePattern: failed to parse start DateTime "
                   "from JSON";

        case Error::JSONFailedToParseInterval:
            return "IntervalRecurrencePattern: failed to parse Interval from "
                   "JSON";

        case Error::InvalidInterval:
            return "IntervalRecurrencePattern: cannot instantiate recurrence "
                   "pattern from zero Interval";

        default:
            std::unreachable();
        }
    }

  private:
    static constexpr auto jsonStartField{std::string_view{"start"}};
    static constexpr auto jsonIntervalField{std::string_view{"interval"}};

    static constexpr auto jsonFields{
        std::array<std::string_view, 2>{jsonStartField, jsonIntervalField}};

  private:
    static auto validateInterval(const Interval &interval) -> Interval;

  private:
    DateTime start_;
    Interval interval_;

  public:
    IntervalRecurrencePattern(DateTime start, Interval interval);

  public:
    [[nodiscard]] auto getInterval() const -> Interval;

  public:
    [[nodiscard]] auto happensOnDate(DateTime on) const -> bool;

  public:
    [[nodiscard]] auto getFirstOccurrenceOfDate(DateTime on) const
        -> std::optional<occurrence_t>;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime on) const
        -> occurrences_t override;

  public:
    [[nodiscard]] auto operator==(const IntervalRecurrencePattern &other) const
        -> bool;

  private:
    [[nodiscard]] static auto containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::expected<IntervalRecurrencePattern, Error>;
};
} // namespace hbt::mods::util
