#pragma once

#include <datetime.hpp>
#include <recurrence.hpp>
#include <recurrence_patterns.hpp>
#include <singular_task.hpp>
#include <task_data.hpp>
#include <task_override.hpp>
#include <uuid.hpp>

#include <optional>

namespace hbt::mods {
class TaskSeries {
  public:
    using occurrences_t = util::RecurrencePattern::occurrences_t;

    using deadline_t = TaskData::deadline_t;
    using start_t = TaskData::datetime_t;

    using stop_t = std::optional<hbt::mods::DateTime>;
    using uuid_t = core::uuid::uuid_t;

    enum class Error : uint8_t {
        JSONMissingRequiredField,

        JSONFailedToParseTaskData,

        InvalidDeadline,
        InvalidStart,
        InvalidStop,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "TaskSeries: missing required field(s) in JSON";

        case Error::JSONFailedToParseTaskData:
            return "TaskSeries: failed to parse TaskData from JSON";

        case Error::InvalidDeadline:
            return "TaskSeries: only interval-based deadlines are supported";

        case Error::InvalidStart:
            return "TaskSeries: provided start datetime is after series stop";

        case Error::InvalidStop:
            return "TaskSeries: provided stop datetime is before series start";
        }
    }

  private:
    TaskData task_;

    stop_t stop_;
    uuid_t uuid_;

    util::Recurrence recurrence_;

  private:
    auto validateDeadline(deadline_t deadline) const -> deadline_t;

    auto validateStart(start_t start) const -> start_t;

    auto validateStop(stop_t stop) const -> stop_t;

  public:
    TaskSeries(TaskData taskData, util::Recurrence recurrence,
               stop_t stop = std::nullopt);

  public:
    [[nodiscard]] auto getStart() const -> start_t;

    [[nodiscard]] auto getStop() const -> stop_t;

    [[nodiscard]] auto getRecurrence() const -> util::Recurrence;

    [[nodiscard]] auto getUUID() const -> uuid_t;

  public:
    auto setStart(start_t start) -> void;

    auto setStop(stop_t stop) -> void;

    auto setDeadline(deadline_t deadline) -> void;

    auto setRecurrence(util::Recurrence recurrence) -> void;

  private:
    [[nodiscard]] auto generateFirstSingularOfDate(mods::Date date) const
        -> std::optional<hbt::mods::SingularTask>;

  public:
    [[nodiscard]] auto generateSingularsForDate(mods::Date date) const
        -> std::vector<hbt::mods::SingularTask>;

  public:
    [[nodiscard]] auto isForDate(hbt::mods::Date date) const -> bool;
};
} // namespace hbt::mods
