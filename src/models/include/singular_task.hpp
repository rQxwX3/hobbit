#pragma once

#include <datetime.hpp>
#include <deadline.hpp>
#include <task_data.hpp>

#include <array>
#include <string>

namespace hbt::mods {
class SingularTask {
  public:
    enum class Error : uint8_t {
        InvalidDateTime,
        InvalidDeadline,

        JSONMissingRequiredField,

        JSONFailedToParseTaskData,
        JSONFailedToParseDateTime,
        JSONFailedToParseDeadline,

        JSONInvalidDateTimeDeadline,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::InvalidDateTime:
            return "SingularTask: provided datetime is invalid";

        case Error::InvalidDeadline:
            return "SingularTask: provided deadline is invalid";

        case Error::JSONMissingRequiredField:
            return "SingularTask: missing required field(s) in JSON";

        case Error::JSONFailedToParseTaskData:
            return "SingularTask: failed to parse TaskData from JSON";

        case Error::JSONFailedToParseDateTime:
            return "SingularTask: failed to parse DateTime from JSON";

        case Error::JSONFailedToParseDeadline:
            return "SingularTask: failed to parse Deadline from JSON";

        case Error::JSONInvalidDateTimeDeadline:
            return "SingularTask: provided JSON contains deadline that is "
                   "earlier than (or equal to) the datetime";

        default:
            std::unreachable();
        }
    }

  private:
    static constexpr auto jsonTaskField{std::string_view{"task"}};
    static constexpr auto jsonDateTimeField{std::string_view{"datetime"}};
    static constexpr auto jsonDeadlineField{std::string_view{"deadline"}};

    static constexpr auto jsonFields{std::array<std::string_view, 3>{
        jsonTaskField, jsonDateTimeField, jsonDeadlineField}};

  private:
    TaskData task_;

    /* order must not be changed */
    DateTime datetime_;
    Deadline deadline_;

  private:
    auto validateDateTime(DateTime datetime) -> DateTime;

    auto validateDeadline(Deadline deadline) -> Deadline;

    static auto validateDateTimeDeadline(DateTime datetime, Deadline deadline)
        -> void;

  public:
    SingularTask(TaskData task, DateTime datetime, Deadline deadline);

  public:
    auto setTitle(std::string title) -> void;

    auto setDateTime(DateTime datetime) -> void;

    auto setDeadline(Deadline deadline) -> void;

    auto setCompleted(bool completed) -> void;

  public:
    [[nodiscard]] auto getTitle() const -> std::string_view;

    [[nodiscard]] auto getDateTime() const -> DateTime;

    [[nodiscard]] auto getDeadline() const -> Deadline;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    [[nodiscard]] virtual auto isForDate(DateTime datetime) const -> bool;

    [[nodiscard]] auto hasDeadline() const -> bool;

  public:
    [[nodiscard]] auto operator==(const SingularTask &other) const -> bool;

  private:
    [[nodiscard]] auto static containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<SingularTask, Error>;
};
} // namespace hbt::mods
