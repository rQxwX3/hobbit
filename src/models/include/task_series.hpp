#pragma once

#include <datetime.hpp>
#include <deadline.hpp>
#include <instance.hpp>
#include <null_pattern.hpp>
#include <opt_datetime.hpp>
#include <recurrence.hpp>
#include <task_override.hpp>
#include <uuid.hpp>

namespace hbt::mods {
class TaskSeries {
  public:
    using Recurrence = mods::util::Recurrence;
    using Deadline = mods::Deadline;
    using DateTime = mods::DateTime;
    using Date = mods::Date;
    using OptDateTime = util::OptDateTime;
    using uuid_t = core::uuid::uuid_t;

    enum class Error : uint8_t {
        TitleEmpty,

        RecurrenceIncompatibleWithDeadline,

        DeadlineIncompatibleWithRecurrence,
        DeadlineBeforeStartDateTime,
    };

  public:
    static auto
    rethrowRecurrenceInvalidArgumentException(const std::exception &exception)
        -> void {
        throw std::invalid_argument("task::Template: " +
                                    std::string(exception.what()));
    }

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::TitleEmpty:
            return "task::Template: Template's title cannot be empty";

        case Error::RecurrenceIncompatibleWithDeadline:
            return "task::Template: a Template with DateTime deadline cannot "
                   "be recurrent";

        case Error::DeadlineIncompatibleWithRecurrence:
            return "task::Template: provided Deadline is not compatible with "
                   "Template's recurrence pattern";

        case Error::DeadlineBeforeStartDateTime:
            return "task::Template: Template's deadline cannot appear before "
                   "its datetime";

        default:
            std::unreachable();
        }
    }

  private:
    /* order must not be changed */
    uuid_t uuid_;
    std::string title_;
    Deadline deadline_;
    Recurrence recurrence_;

  public:
    TaskSeries(std::string title, Recurrence recurrence = Recurrence::null(),
               Deadline deadline = Deadline::null());

  public:
    [[nodiscard]] auto happensOnDate(Date date) const -> bool;

    [[nodiscard]] auto generateInstancesForDate(Date date) const
        -> std::vector<task::Instance>;

  public:
    [[nodiscard]] auto getUUID() const -> uuid_t;

    [[nodiscard]] auto getTitle() const -> std::string;

    [[nodiscard]] auto getRecurrence() const -> Recurrence;

    [[nodiscard]] auto getDeadline() const -> Deadline;

    [[nodiscard]] auto getStartDateTime() const -> DateTime;

    [[nodiscard]] auto getEndDateTime() const -> OptDateTime;

  public:
    auto setTitle(const std::string &title) -> void;

    auto setRecurrence(const Recurrence &recurrence) -> void;

    auto setDeadline(const Deadline &deadline) -> void;

    auto setStartDateTime(const DateTime &startDateTime) -> void;

    auto setEndDateTime(const OptDateTime &endDateTime) -> void;

  private:
    struct Validator {
        // TODO: add validation for UUID

        struct Validated {};

        static auto title(const std::string &title) -> void;

        static auto
        recurrenceCompatibleWithDeadline(const Recurrence &recurrence,
                                         const Deadline &deadline) -> void;

        static auto
        deadlineCompatibleWithRecurrence(const Deadline &deadline,
                                         const Recurrence &recurrence) -> void;

        struct Return {
            [[nodiscard]] static auto title(const std::string &title)
                -> std::string;

            [[nodiscard]] static auto recurrence(const Recurrence &recurrence,
                                                 const Deadline &deadline)
                -> Recurrence;

            [[nodiscard]] static auto deadline(const Deadline &deadline,
                                               const Recurrence &recurrence)
                -> Deadline;
        };
    };

  private:
    [[nodiscard]] static auto fromValidated(uuid_t uuid, std::string title,
                                            util::Recurrence recurrence,
                                            Deadline deadline) -> TaskSeries;

    TaskSeries(Validator::Validated, uuid_t uuid, std::string title,
               util::Recurrence recurrence, Deadline deadline);

  public:
    struct JSON {
        enum class Error : uint8_t {
            MissingRequiredField,

            FailedToParseRecurrence,
            FailedToParseDeadline,

            FailedToValidateTitle,
            FailedToValidateDeadline
        };

        [[nodiscard]] static constexpr auto errorMessage(JSON::Error error)
            -> std::string {
            switch (error) {
            case Error::MissingRequiredField:
                return "task::Template::JSON: missing required field(s)";

            case Error::FailedToParseRecurrence:
                return "task::Template::JSON: failed to parse Recurrence";

            case Error::FailedToParseDeadline:
                return "task::Template::JSON: failed to parse Deadline";

            case Error::FailedToValidateTitle:
                return "task::Template::JSON: failed to validate Title";

            case Error::FailedToValidateDeadline:
                return "task::Template::JSON: failed to validate Deadline";

            default:
                std::unreachable();
            }
        }

        static constexpr auto titleField{std::string_view{"title"}};
        static constexpr auto recurrenceField{std::string_view{"recurrence"}};
        static constexpr auto deadlineField{std::string_view{"deadline"}};
        static constexpr auto uuidField{std::string_view{"uuid"}};

        static constexpr auto fields{std::array<std::string_view, 4>{
            titleField, recurrenceField, deadlineField, uuidField}};

        [[nodiscard]] static auto containsAllFields(const nlohmann::json &json)
            -> bool;

        [[nodiscard]] static auto encode(const TaskSeries &taskSeries)
            -> nlohmann::json;

        [[nodiscard]] static auto decode(const nlohmann::json &json)
            -> std::expected<TaskSeries, Error>;
    };
};
} // namespace hbt::mods
