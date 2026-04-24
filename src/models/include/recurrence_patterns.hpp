#pragma once

#include <interval.hpp>
#include <weekdays.hpp>

#include <nlohmann/json.hpp>

#include <optional>

namespace hbt::mods::util {
class RecurrencePattern {
  public:
    using occurrence_t = hbt::mods::DateTime;
    using occurrences_t = std::vector<occurrence_t>;

  public:
    RecurrencePattern() = default;

  public:
    virtual ~RecurrencePattern() = default;

  public:
    [[nodiscard]] virtual auto getOccurrencesOnDate(mods::DateTime start,
                                                    mods::Date date) const
        -> occurrences_t = 0;
};

class IntervalRecurrencePattern : public RecurrencePattern {
  private:
    static constexpr auto typeJSON{std::string{"interval"}};

  private:
    hbt::mods::Interval interval_;

  private:
    [[nodiscard]] auto static isValidJSON(const nlohmann::json &json) -> bool;

  public:
    IntervalRecurrencePattern(const hbt::mods::Interval &interval);

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::optional<IntervalRecurrencePattern>;

  public:
    [[nodiscard]] auto getInterval() const -> hbt::mods::Interval;

  public:
    [[nodiscard]] auto happensOnDate(hbt::mods::DateTime start,
                                     hbt::mods::Date date) const -> bool;

  private:
    [[nodiscard]] auto getFirstOccurrencesOnDate(mods::DateTime start,
                                                 mods::Date date) const
        -> std::optional<occurrence_t>;

  public:
    [[nodiscard]] auto getOccurrencesOnDate(mods::DateTime start,
                                            mods::Date date) const
        -> occurrences_t override;
};

class WeekdayRecurrencePattern : public RecurrencePattern {
  private:
    static constexpr auto typeJSON{std::string{"weekday"}};

  private:
    static constexpr auto invalidIntervalError{
        std::string_view{"WeekdayRecurrence: provided interval contains units "
                         "others than week"}};

  private:
    hbt::mods::Interval interval_;
    hbt::mods::Weekdays weekdays_;

  private:
    [[nodiscard]] auto static isValidJSON(const nlohmann::json &json) -> bool;

  private:
    [[nodiscard]] auto getDateOfFirstOccurrence(mods::DateTime start) const
        -> occurrence_t;

  public:
    WeekdayRecurrencePattern(const hbt::mods::Interval &interval,
                             hbt::mods::Weekdays weekdays);

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::optional<WeekdayRecurrencePattern>;

  public:
    [[nodiscard]] auto getInterval() const -> hbt::mods::Interval;

    [[nodiscard]] auto getWeekdays() const -> hbt::mods::Weekdays;

  public:
    [[nodiscard]] auto happensOnDate(hbt::mods::DateTime start,
                                     hbt::mods::Date date) const -> bool;

  public:
    [[nodiscard]] auto getOccurrencesOnDate(mods::DateTime start,
                                            mods::Date date) const
        -> occurrences_t override;
};
} // namespace hbt::mods::util
