#pragma once

#include <datetime.hpp>
#include <deadline.hpp>

#include <array>
#include <expected>
#include <optional>
#include <string>

namespace hbt::mods {
class TaskData {
  public:
    using deadline_t = std::optional<Deadline>;
    using datetime_t = DateTime;

  private:
    enum class Error : uint8_t {
        JSONMissingRequiredField,

        JSONFailedToParseDateTime,
        JSONFailedToParseDeadline,

        InvalidDeadline,
        InvalidDateTime,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "TaskData: missing required field(s) in JSON";

        case Error::JSONFailedToParseDateTime:
            return "TaskData: failed to parse DateTime from JSON";

        case Error::JSONFailedToParseDeadline:
            return "TaskData: failed to parse Deadline from JSON";

        case Error::InvalidDeadline:
            return "TaskData: provided Deadline is before task's Datetime";

        case Error::InvalidDateTime:
            return "TaskData: provided Datetime is after task's Deadline";
        }
    }

  private:
    static constexpr auto jsonTitleField{std::string_view{"title"}};
    static constexpr auto jsonDateTimeField{std::string_view{"datetime"}};
    static constexpr auto jsonDeadlineField{std::string_view{"deadline"}};
    static constexpr auto jsonCompletedField{std::string_view{"completed"}};

    static constexpr auto jsonFields{
        std::array<std::string_view, 4>{jsonTitleField, jsonDateTimeField,
                                        jsonDeadlineField, jsonCompletedField}};

    static constexpr auto jsonNullDeadlineValue{std::string_view{"none"}};

  private:
    std::string title_;

    datetime_t datetime_;
    deadline_t deadline_;

    bool completed_;

  private:
    auto validateDateTime(datetime_t datetime) const -> datetime_t;

    auto validateDeadline(deadline_t deadline) const -> deadline_t;

  public:
    TaskData(std::string title, datetime_t datetime, bool completed = false,
             deadline_t deadline = std::nullopt);

  public:
    [[nodiscard]] auto getTitle() const -> std::string_view;

    [[nodiscard]] auto getDateTime() const -> datetime_t;

    [[nodiscard]] auto getDeadline() const -> deadline_t;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    auto setTitle(std::string title) -> void;

    auto setDateTime(datetime_t datetime) -> void;

    auto setDeadline(deadline_t deadline) -> void;

    auto setCompleted(bool completed) -> void;

  private:
    [[nodiscard]] static auto deadlineFromJSON(const nlohmann::json &json)
        -> std::expected<deadline_t, Error>;

    [[nodiscard]] static auto containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<TaskData, Error>;
};
} // namespace hbt::mods
