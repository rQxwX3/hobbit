#pragma once

#include <datetime.hpp>
#include <recurrence.hpp>
#include <singular_task.hpp>
#include <task_data.hpp>
#include <task_override.hpp>

#include <optional>

namespace hbt::mods {
class TaskSeries {
  public:
    using endDateTime_t = std::optional<DateTime>;

    enum class Error : uint8_t {
        InvalidDeadline,
        InvalidStartDateTime,
        InvalidEndDateTime,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidDeadline:
            return "TaskSeries: only interval-based Deadlines are allowed";

        case Error::InvalidStartDateTime:
            return "TaskSeries: start DateTime must be earlier than end "
                   "DateTime";

        case Error::InvalidEndDateTime:
            return "TaskSeries: end DateTime must be later than start DateTime";

        default:
            std::unreachable();
        }
    }

  private:
    TaskData taskData_;
    util::Recurrence recurrence_;

    endDateTime_t endDateTime_;

  private:
    static auto
    rethrowTaskDataInvalidArgumentException(const std::exception &exception)
        -> void {
        throw std::invalid_argument("TaskSeries: " +
                                    std::string(exception.what()));
    }

  private:
    static auto validateDeadline(Deadline deadline) -> Deadline;

    auto validateStartDateTime(DateTime startDateTime) const -> DateTime;

    auto validateEndDateTime(endDateTime_t endDateTime) const -> endDateTime_t;

  public:
    TaskSeries(TaskData taskData, util::Recurrence recurrence,
               endDateTime_t endDateTime = std::nullopt);

  public:
    [[nodiscard]] auto getTaskData() const -> TaskData;

    [[nodiscard]] auto getTitle() const -> std::string;

    [[nodiscard]] auto getStartDateTime() const -> DateTime;

    [[nodiscard]] auto getDeadline() const -> Deadline;

    [[nodiscard]] auto getRecurrence() const -> util::Recurrence;

    [[nodiscard]] auto getEndDateTime() const -> endDateTime_t;

  public:
    auto setTitle(std::string title) -> void;

    auto setStartDateTime(DateTime startDateTime) -> void;

    auto setDeadline(Deadline deadline) -> void;

    auto setRecurrence(util::Recurrence recurrence) -> void;

    auto setEndDateTime(endDateTime_t endDateTime) -> void;

  public:
    [[nodiscard]] auto happensOnDate(DateTime datetime) const -> bool;

    [[nodiscard]] auto generateSingularsForDate(DateTime datetime) const
        -> std::vector<SingularTask>;

  public:
    struct JSON {
        enum class Error : uint8_t {
            MissingRequiredField,

            FailedToParseTaskData,
            FailedToParseEndDateTime,
            FailedToParseRecurrence,
        };

        [[nodiscard]] static constexpr auto errorMessage(JSON::Error error)
            -> std::string {
            switch (error) {
            case Error::MissingRequiredField:
                return "TaskSeries::JSON: missing required field(s)";

            case Error::FailedToParseTaskData:
                return "TaskSeries::JSON: failed to parse TaskData";

            case Error::FailedToParseRecurrence:
                return "TaskSeries::JSON: failed to parse Recurrence";

            case Error::FailedToParseEndDateTime:
                return "TaskSeries::JSON: failed to parse end DateTime";

            default:
                std::unreachable();
            }
        }

        static constexpr auto taskDataField{std::string_view{"task_data"}};
        static constexpr auto endDateTimeField{
            std::string_view{"end_datetime"}};
        static constexpr auto recurrenceField{std::string_view{"recurrence"}};

        static constexpr auto endDateTimeNullValue{std::string_view{"null"}};

        static constexpr auto fields{std::array<std::string_view, 3>{
            taskDataField, endDateTimeField, recurrenceField}};

        [[nodiscard]] static auto containsAllFields(const nlohmann::json &json)
            -> bool;

        [[nodiscard]] static auto encode(const TaskSeries &taskSeries)
            -> nlohmann::json;

        [[nodiscard]] static auto decode(const nlohmann::json &json)
            -> std::expected<TaskSeries, Error>;
    };
};
} // namespace hbt::mods
