#pragma once

#include <recurrence_pattern.hpp>

namespace hbt::mods::util {
class WeekdaysRecurrencePattern : public RecurrencePattern {
  public:
    enum class Error : uint8_t {
        JSONMissingRequiredField,

        JSONFailedToParseFirstWeek,
        JSONFailedToParseInterval,

        JSONInvalidInterval,
        JSONInvalidFirstWeek,

        JSONFirstWeekNotArray,
        JSONFirstWeekInvalidCount,
        JSONFirstWeekArrayIsNotOfStrings,
        JSONFirstWeekFailedToParseDateTime,

        InvalidInterval,
        InvalidFirstWeek,
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

        case Error::InvalidInterval:
            return "WeekdayRecurrencePattern: provided Interval contains units "
                   "other than week";

        default:
            std::unreachable();
        }
    }

  private:
    static constexpr auto jsonFirstWeekField{
        std::string_view{"first_week_datetimes"}};
    static constexpr auto jsonIntervalField{std::string_view{"interval"}};

    static constexpr auto jsonFirstWeekNullValue{"null"};

    static constexpr auto jsonFields{
        std::array<std::string_view, 2>{jsonFirstWeekField, jsonIntervalField}};

  private:
    using firstWeek_t = std::array<std::optional<DateTime>,
                                   static_cast<size_t>(Weekdays::Week::COUNT_)>;

    [[nodiscard]] static auto createFirstWeek(DateTime start, Weekdays weekdays)
        -> firstWeek_t {
        auto result{firstWeek_t{}};
        auto endDT{start + Interval::weeks(1)};

        for (auto dt{start}; dt.getDate() != endDT.getDate();
             dt += Interval::days(1)) {
            auto dtWeekday{dt.getWeekday()};

            if (weekdays.containsWeekday(dtWeekday)) {
                result[static_cast<size_t>(dtWeekday)] = dt;
            }
        }

        return result;
    }

  private:
    firstWeek_t firstWeek_;
    Interval interval_;

  private:
    static auto validateInterval(const Interval &interval) -> Interval;

    static auto validateFirstWeek(const firstWeek_t &firstWeek) -> firstWeek_t;

  public:
    WeekdaysRecurrencePattern(DateTime start, Weekdays weekdays,
                              Interval interval);

    WeekdaysRecurrencePattern(firstWeek_t firstWeek, Interval interval);

  public:
    [[nodiscard]] auto getInterval() const -> Interval;

    [[nodiscard]] auto getWeekdays() const -> Weekdays;

  public:
    [[nodiscard]] auto happensOnDate(DateTime on) const -> bool;

  public:
    [[nodiscard]] auto getOccurrencesOfDate(DateTime on) const
        -> occurrences_t override;

  private:
    [[nodiscard]] auto static containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  private:
    [[nodiscard]] auto firstWeekToJSON() const -> nlohmann::json;

    [[nodiscard]] auto static firstWeekFromJSON(const nlohmann::json &json)
        -> std::expected<firstWeek_t, Error>;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::expected<WeekdaysRecurrencePattern, Error>;
};
} // namespace hbt::mods::util
