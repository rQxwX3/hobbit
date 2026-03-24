#pragma once

#include <nlohmann/json.hpp>

#include <date.hpp>
#include <interval.hpp>

#include <optional>

namespace hbt::mods {

class Occurrence {
  private:
    hbt::mods::Date date_;

    hbt::mods::Interval interval_;

  public:
    Occurrence(hbt::mods::Date date = hbt::mods::Date{},
               hbt::mods::Interval = hbt::mods::Interval{});

  public:
    [[nodiscard]] auto getDate() const -> hbt::mods::Date;

    [[nodiscard]] auto getInterval() const -> hbt::mods::Interval;

    [[nodiscard]] auto getWeekday() const -> hbt::mods::Date::weekday_t;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<Occurrence>;

  public:
    [[nodiscard]] auto isForDate(Date date) const -> bool;

  public:
    [[nodiscard]] auto operator==(const Occurrence &other) const -> bool;
};
} // namespace hbt::mods
