#pragma once

#include <nlohmann/json.hpp>

#include <date.hpp>
#include <interval.hpp>

#include <optional>

namespace hbt::mods {

class Occurrence {
  private:
    template <typename T>
    [[nodiscard]] auto compareOptional(const std::optional<T> &first,
                                       const std::optional<T> &second) const
        -> bool {
        if (first.has_value() && !second.has_value()) {
            return false;
        }
        if (!first.has_value() && second.has_value()) {
            return false;
        }
        if (!first.has_value() && !second.has_value()) {
            return true;
        }
        return first.value() == second.value();
    }

  private:
    using interval_t = std::optional<hbt::mods::Interval>;

  private:
    hbt::mods::Date date_;

    interval_t interval_;

  public:
    Occurrence();

    Occurrence(hbt::mods::Date date);

    Occurrence(hbt::mods::Date date, interval_t interval);

  public:
    [[nodiscard]] auto getDate() const -> hbt::mods::Date;

    [[nodiscard]] auto getInterval() const -> interval_t;

    [[nodiscard]] auto getWeekday() const -> hbt::mods::Date::weekday_t;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> Occurrence;

  public:
    [[nodiscard]] auto isForDate(Date date) const -> bool;

  public:
    [[nodiscard]] auto operator==(const Occurrence &other) const -> bool;
};
} // namespace hbt::mods
