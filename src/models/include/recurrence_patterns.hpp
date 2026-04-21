#pragma once

#include <interval.hpp>
#include <weekdays.hpp>

#include <nlohmann/json.hpp>

#include <optional>

namespace hbt::mods::util {

class IntervalRecurrence {
  public:
    using occurrence_t = hbt::mods::DateTime;
    using occurrences_t = std::vector<occurrence_t>;

  private:
    static constexpr auto typeJSON{std::string{"interval"}};

  private:
    hbt::mods::Interval interval_;

  private:
    [[nodiscard]] auto static isValidJSON(const nlohmann::json &json) -> bool;

  public:
    IntervalRecurrence(const hbt::mods::Interval &interval);

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::optional<IntervalRecurrence>;

  public:
    [[nodiscard]] auto getInterval() const -> hbt::mods::Interval;

  public:
    [[nodiscard]] auto happensOnDate(hbt::mods::DateTime startDate,
                                     hbt::mods::DateTime datetime) const
        -> bool;

  private:
    [[nodiscard]] auto getFirstOccurrencesOnDate(mods::DateTime start,
                                                 mods::DateTime datetime) const
        -> std::optional<occurrence_t>;

  public:
    [[nodiscard]] auto getOccurrencesOnDate(mods::DateTime start,
                                            mods::DateTime datetime) const
        -> occurrences_t;
};

class WeekdayRecurrence {
  public:
    using occurrence_t = hbt::mods::DateTime;
    using occurrences_t = std::vector<occurrence_t>;

  private:
    static constexpr auto typeJSON{std::string{"weekday"}};

  private:
    hbt::mods::Interval interval_;
    hbt::mods::Weekdays weekdays_;

  private:
    [[nodiscard]] auto static isValidJSON(const nlohmann::json &json) -> bool;

  private:
    [[nodiscard]] auto getDateOfFirstOccurrence(mods::DateTime start) const
        -> occurrence_t;

  public:
    WeekdayRecurrence(const hbt::mods::Interval &interval,
                      hbt::mods::Weekdays weekdays);

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] auto static fromJSON(const nlohmann::json &json)
        -> std::optional<WeekdayRecurrence>;

  public:
    [[nodiscard]] auto getInterval() const -> hbt::mods::Interval;

    [[nodiscard]] auto getWeekdays() const -> hbt::mods::Weekdays;

  public:
    [[nodiscard]] auto happensOnDate(hbt::mods::DateTime start,
                                     hbt::mods::DateTime datetime) const
        -> bool;

  public:
    [[nodiscard]] auto getOccurrencesOnDate(mods::DateTime start,
                                            mods::DateTime datetime) const
        -> occurrences_t;
};
} // namespace hbt::mods::util
