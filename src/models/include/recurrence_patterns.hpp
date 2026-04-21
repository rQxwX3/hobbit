#pragma once

#include <interval.hpp>
#include <weekdays.hpp>

#include <nlohmann/json.hpp>

#include <optional>

namespace hbt::mods::util {
class IntervalRecurrence {
  private:
    static constexpr auto typeJSON{std::string{"interval"}};

  private:
    hbt::mods::Interval interval_;

  private:
    [[nodiscard]] auto static isValidJSON(const nlohmann::json &json) -> bool;

  public:
    IntervalRecurrence(hbt::mods::Interval interval);

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
};

class WeekdayRecurrence {
  private:
    static constexpr auto typeJSON{std::string{"weekday"}};

  private:
    hbt::mods::Interval interval_;
    hbt::mods::Weekdays weekdays_;

  private:
    [[nodiscard]] auto static isValidJSON(const nlohmann::json &json) -> bool;

  private:
    [[nodiscard]] auto getDateOfFirstOccurrence(mods::DateTime startDate) const
        -> hbt::mods::DateTime;

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
    [[nodiscard]] auto happensOnDate(hbt::mods::DateTime startDate,
                                     hbt::mods::DateTime datetime) const
        -> bool;
};
} // namespace hbt::mods::util
