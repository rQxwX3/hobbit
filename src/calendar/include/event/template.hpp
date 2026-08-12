#pragma once

#include <datetime/datetime.hpp>
#include <datetime/opt_datetime.hpp>
#include <event/deadline.hpp>
#include <event/instance.hpp>
#include <recurrence/null_pattern.hpp>
#include <recurrence/recurrence.hpp>
#include <uuid.hpp>

namespace clndr::ev {
class Template {
  public:
    static auto
    rethrowRecurrenceInvalidArgumentException(const std::exception &exception)
        -> void {
        throw std::invalid_argument("task::Template: " +
                                    std::string(exception.what()));
    }

  private:
    /* order must not be changed */
    core::uuid_t uuid_;
    std::string title_;
    ev::Deadline deadline_;
    rec::Recurrence recurrence_;

  public:
    Template();

    Template(std::string title,
             rec::Recurrence recurrence = rec::Recurrence::null(),
             ev::Deadline deadline = ev::Deadline::null());

  public:
    [[nodiscard]] auto happensOnDate(dt::Date date) const -> bool;

    [[nodiscard]] auto generateInstancesForDate(dt::Date date) const
        -> std::vector<ev::Instance>;

  public:
    [[nodiscard]] auto getUUID() const -> core::uuid_t;

    [[nodiscard]] auto getTitle() const -> std::string;

    [[nodiscard]] auto getRecurrence() const -> rec::Recurrence;

    [[nodiscard]] auto getDeadline() const -> ev::Deadline;

    [[nodiscard]] auto getStartDateTime() const -> dt::DateTime;

    [[nodiscard]] auto getEndDateTime() const -> dt::OptDateTime;

  public:
    auto setTitle(const std::string &title) -> void;

    auto setRecurrence(const rec::Recurrence &recurrence) -> void;

    auto setDeadline(const ev::Deadline &deadline) -> void;

    auto setStartDateTime(const dt::DateTime &startDateTime) -> void;

    auto setEndDateTime(const dt::OptDateTime &endDateTime) -> void;

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

        [[nodiscard]] static auto encode(const Template &taskSeries)
            -> nlohmann::json;

        [[nodiscard]] static auto decode(const nlohmann::json &json)
            -> std::expected<Template, Error>;
    };
};
} // namespace clndr::ev
