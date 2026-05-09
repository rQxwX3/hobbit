#pragma once

#include <datetime.hpp>
#include <interval.hpp>
#include <weekdays.hpp>

#include <nlohmann/json.hpp>

namespace hbt::mods::util {
class RecurrencePattern {
  public:
    using occurrence_t = hbt::mods::DateTime;
    using occurrences_t = std::vector<occurrence_t>;

  public:
    RecurrencePattern() = default;

  public:
    RecurrencePattern(const RecurrencePattern &) = default;
    auto operator=(const RecurrencePattern &) -> RecurrencePattern & = default;

    RecurrencePattern(RecurrencePattern &&) = delete;
    auto operator=(RecurrencePattern &&) -> RecurrencePattern & = delete;

    [[nodiscard]] auto operator==(const RecurrencePattern &other) const
        -> bool = default;

  public:
    virtual ~RecurrencePattern() = default;

  public:
    [[nodiscard]] virtual auto getOccurrencesOfDate(DateTime on) const
        -> occurrences_t = 0;
};
} // namespace hbt::mods::util
