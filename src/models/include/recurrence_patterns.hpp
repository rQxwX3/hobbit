#pragma once

#include <datetime.hpp>
#include <interval.hpp>
#include <weekdays.hpp>

#include <nlohmann/json.hpp>

#include <array>
#include <expected>
#include <string_view>

namespace hbt::mods::util {
class RecurrencePattern {
  public:
    using timestamp_t = hbt::mods::DateTime;
    using timestamps_t = std::vector<timestamp_t>;

  public:
    RecurrencePattern() = default;

  public:
    RecurrencePattern(const RecurrencePattern &) = default;
    auto operator=(const RecurrencePattern &) -> RecurrencePattern & = default;

    RecurrencePattern(RecurrencePattern &&) = delete;
    auto operator=(RecurrencePattern &&) -> RecurrencePattern & = delete;

  public:
    virtual ~RecurrencePattern() = default;

  public:
    [[nodiscard]] virtual auto getTimeStampsOnDate(DateTime start,
                                                   DateTime on) const
        -> timestamps_t = 0;
};

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
    [[nodiscard]] auto getFirstTimeStampOnDate(DateTime start,
                                               DateTime on) const
        -> std::optional<timestamp_t>;

  public:
    [[nodiscard]] auto getTimeStampsOnDate(DateTime start, DateTime on) const
        -> timestamps_t override;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::expected<IntervalRecurrencePattern, Error>;
};

class WeekdayRecurrencePattern : public RecurrencePattern {
  private:
    enum class Error : uint8_t {
        JSONMissingRequiredField,

        JSONFailedToParseInterval,
        JSONFailedToParseWeekdays,

        InvalidInterval,
        InvalidWeekdays,

        EmptyWeekdays,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "WeekdayRecurrencePattern: missing required field(s) in "
                   "JSON";

        case Error::JSONFailedToParseInterval:
            return "WeekdayRecurrencePattern: failed to parse Interval from "
                   "JSON";

        case Error::JSONFailedToParseWeekdays:
            return "WeekdayRecurrencePattern: failed to parse Weekdays from "
                   "JSON";

        case Error::InvalidInterval:
            return "WeekdayRecurrencePattern: provided Interval contains units "
                   "other than week";

        case Error::InvalidWeekdays:
            return "WeekdayRecurrencePattern: provided Weekdays do not contain "
                   "single selected day";

        case Error::EmptyWeekdays:
            return "WeekdayRecurrencePattern: invalid object state (empty "
                   "Weekdays object)";

        default:
            return "WeekdayRecurrencePattern: unclassified error";
        }
    }

  private:
    static constexpr auto jsonIntervalField{std::string_view{"interval"}};
    static constexpr auto jsonWeekdaysField{std::string_view{"weekdays"}};

    static constexpr auto jsonFields{
        std::array<std::string_view, 2>{jsonIntervalField, jsonWeekdaysField}};

  private:
    Interval interval_;
    Weekdays weekdays_;

  private:
    [[nodiscard]] auto getDateOfFirstTimeStamp(DateTime start) const
        -> timestamp_t;

    static auto validateInterval(const Interval &interval) -> Interval;

    static auto validateWeekdays(Weekdays weekdays) -> Weekdays;

  public:
    WeekdayRecurrencePattern(const Interval &interval, Weekdays weekdays);

  public:
    [[nodiscard]] auto getInterval() const -> Interval;

    [[nodiscard]] auto getWeekdays() const -> Weekdays;

  public:
    [[nodiscard]] auto happensOnDate(DateTime start, DateTime on) const -> bool;

  public:
    [[nodiscard]] auto getTimeStampsOnDate(DateTime start, DateTime on) const
        -> timestamps_t override;

  private:
    [[nodiscard]] auto static containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::expected<WeekdayRecurrencePattern, Error>;
};
} // namespace hbt::mods::util
