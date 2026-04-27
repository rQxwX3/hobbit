#pragma once

#include <datetime.hpp>
#include <recurrence.hpp>
#include <recurrence_patterns.hpp>
#include <singular_task.hpp>
#include <task_data.hpp>
#include <task_override.hpp>

#include <optional>

namespace hbt::mods {
class TaskSeries {
  public:
    using occurrences_t = util::RecurrencePattern::occurrences_t;

    using deadline_t = TaskData::deadline_t;
    using start_t = TaskData::datetime_t;

    using stop_t = std::optional<hbt::mods::DateTime>;

    enum class Error : uint8_t {
        JSONMissingRequiredField,

        JSONFailedToParseTaskData,
        JSONFailedToParseStop,
        JSONFailedToParseRecurrence,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "TaskSeries: missing required field(s) in JSON";

        case Error::JSONFailedToParseTaskData:
            return "TaskSeries: failed to parse TaskData from JSON";

        case Error::JSONFailedToParseRecurrence:
            return "TaskSeries: failed to parse Recurrence from JSON";

        case Error::JSONFailedToParseStop:
            return "TaskSeries: failed to parse stop DateTime from JSON";
        }
    }

  private:
    static constexpr auto jsonTaskField{std::string_view{"task"}};
    static constexpr auto jsonStopField{std::string_view{"stop"}};
    static constexpr auto jsonRecurrenceField{std::string_view{"recurrence"}};

    static constexpr auto jsonFields{std::array<std::string_view, 3>{
        jsonTaskField, jsonStopField, jsonRecurrenceField}};

  private:
    TaskData task_;

    stop_t stop_;

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

  private:
    [[nodiscard]] auto static containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<TaskSeries, Error>;
};
} // namespace hbt::mods
