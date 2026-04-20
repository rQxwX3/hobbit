#pragma once

#include <datetime.hpp>
#include <recurrence_patterns.hpp>
#include <task.hpp>

#include <optional>
#include <variant>

namespace hbt::mods {
class RecurrentTask : public Task {
  public:
    virtual ~RecurrentTask() = default;
    using repeatUntil_t = std::optional<hbt::mods::DateTime>;
    using recurrencePattern_t =
        std::variant<hbt::mods::util::IntervalRecurrence,
                     hbt::mods::util::WeekdayRecurrence>;

  private:
    static constexpr auto invalidDeadlineError{std::string_view{
        "Recurrent tasks can only have interval-based deadlines"}};

    static constexpr auto invalidStartFromError{std::string_view{
        "Recurrent tasks can't start after they stop repeating"}};

    static constexpr auto invalidRepeatUntilError{std::string_view{
        "Recurrent tasks can't stop repeating before they start"}};

  private:
    recurrencePattern_t recurrencePattern_;

    repeatUntil_t repeatUntil_;

  public:
    RecurrentTask(std::string title, hbt::mods::DateTime startFrom,
                  recurrencePattern_t recurrencePattern,
                  repeatUntil_t repeatUntil = std::nullopt,
                  deadline_t deadline = std::monostate{},
                  bool isCompleted = false);

  private:
    [[nodiscard]] auto validateDeadline(deadline_t deadline)
        -> deadline_t override;

    [[nodiscard]] auto validateStartFrom(hbt::mods::DateTime startFrom)
        -> hbt::mods::DateTime override;

    [[nodiscard]] auto validateRepeatUntil(repeatUntil_t repeatUntil)
        -> repeatUntil_t;

  public:
    auto setStartFrom(hbt::mods::DateTime startFrom) -> void override;

    auto setDeadline(deadline_t deadline) -> void override;

    auto setRecurrencePattern(recurrencePattern_t recurrencePattern);

    auto setRepeatUntil(repeatUntil_t repeatUntil);

  public:
    [[nodiscard]] auto getRecurrencePattern() const -> recurrencePattern_t;

    [[nodiscard]] auto getRepeatUntil() const -> repeatUntil_t;

  public:
    [[nodiscard]] auto isForDate(hbt::mods::DateTime datetime) const
        -> bool override;
};
} // namespace hbt::mods
