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
    static auto validateTitle(const std::string &title) -> void;

    static auto validateDeadlineAgainstDateTime(const Deadline &deadline,
                                                const DateTime &datetime)
        -> void;

    [[nodiscard]] static auto validateAndReturnTitle(const std::string &title)
        -> std::string;

    [[nodiscard]] auto validateAndReturnDateTime(const DateTime &datetime) const
        -> DateTime;

    [[nodiscard]] auto validateAndReturnDeadline(const Deadline &deadline) const
        -> Deadline;

  private:
    /* avoid double validation when constructing from JSON (as JSON::decode
     * validates each field) */

    struct Validated {}; // tag struct to overload ctor

    TaskData(Validated, std::string title, DateTime datetime,
             Deadline deadline);

    [[nodiscard]] static auto fromValidated(std::string title,
                                            DateTime datetime,
                                            Deadline deadline) -> TaskData;

  private:
    /* order must not be changed */
    std::string title_;
    DateTime datetime_;
    Deadline deadline_;

  public:
    TaskData(std::string title, DateTime datetime = DateTime::now(),
             Deadline deadline = Deadline::null());

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

            FailedToValidateTitle,
            FailedToValidateDeadlineAgainsDateTime,
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

            case Error::FailedToValidateTitle:
                return "TaskData::JSON: failed to validate Title";

            case Error::FailedToValidateDeadlineAgainsDateTime:
                return "TaskData::JSON: failed to validate Deadline against "
                       "DateTime";

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
