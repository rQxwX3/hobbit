#pragma once

#include <datetime.hpp>
#include <deadline.hpp>

#include <array>
#include <expected>
#include <string>

namespace hbt::mods {
class TaskData {
  public:
    enum class Error : uint8_t {
        EmptyTitle,

        InvalidDateTime,
        InvalidDeadline,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::EmptyTitle:
            return "TaskData: provided title is an empty string";

        case Error::InvalidDateTime:
            return "TaskData: datetime cannot be later than deadline";

        case Error::InvalidDeadline:
            return "TaskData: datetime-like deadline cannot be earlier than "
                   "datetime";

        default:
            std::unreachable();
        }
    }

  private:
  private:
    [[nodiscard]] static auto validateTitle(const std::string &title)
        -> std::string;

    [[nodiscard]] auto validateDateTime(const DateTime &datetime) const
        -> DateTime;

    [[nodiscard]] auto validateDeadline(const Deadline &deadline) const
        -> Deadline;

  private:
    /* order must not be changed */
    std::string title_;
    DateTime datetime_;
    Deadline deadline_;

  public:
    TaskData(std::string title, DateTime datetime, Deadline deadline);

  public:
    [[nodiscard]] auto getTitle() const -> std::string_view;

    [[nodiscard]] auto getDateTime() const -> DateTime;

    [[nodiscard]] auto getDeadline() const -> Deadline;

  public:
    auto setTitle(std::string title) -> void;

    auto setDateTime(DateTime datetime) -> void;

    auto setDeadline(Deadline deadline) -> void;

  public:
    [[nodiscard]] auto operator==(const TaskData &other) const
        -> bool = default;

  public:
    struct JSON {
        enum class Error : uint8_t {
            MissingRequiredField,

            FailedToParseDateTime,
            FailedToParseDeadline,
        };

        [[nodiscard]] static constexpr auto errorMessage(Error error)
            -> std::string {
            switch (error) {
            case Error::MissingRequiredField:
                return "TaskData::JSON: missing required field(s)";

            case Error::FailedToParseDateTime:
                return "TaskData::JSON: failed to parse DateTime";

            case Error::FailedToParseDeadline:
                return "TaskData::JSON: failed to parse Deadline";

            default:
                std::unreachable();
            }
        }

        static constexpr auto titleField{std::string_view{"title"}};
        static constexpr auto dateTimeField{std::string_view{"datetime"}};
        static constexpr auto deadlineField{std::string_view{"deadline"}};

        static constexpr auto fields{std::array<std::string_view, 3>{
            titleField, dateTimeField, deadlineField}};

        [[nodiscard]] static auto containsAllFields(const nlohmann::json &json)
            -> bool;

        [[nodiscard]] static auto encode(const TaskData &taskData)
            -> nlohmann::json;

        [[nodiscard]] static auto decode(const nlohmann::json &json)
            -> std::expected<TaskData, JSON::Error>;
    };
};
} // namespace hbt::mods
