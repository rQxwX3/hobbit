#pragma once

#include <datetime.hpp>
#include <deadline.hpp>

#include <array>
#include <string>

namespace hbt::mods {
class SingularTask {
  private:
    bool isCompleted_;

  private:
    static auto
    rethrowTaskDataInvalidArgumentException(const std::exception &exception)
        -> void {
        throw std::invalid_argument("SingularTask: " +
                                    std::string(exception.what()));
    }

  public:
    SingularTask(TaskData taskData, bool isCompleted = false);

  public:
    [[nodiscard]] auto getTaskData() const -> TaskData;

    [[nodiscard]] auto getTitle() const -> std::string_view;

    [[nodiscard]] auto getDateTime() const -> DateTime;

    [[nodiscard]] auto getDeadline() const -> Deadline;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    auto setTitle(std::string title) -> void;

    auto setDateTime(DateTime datetime) -> void;

    auto setDeadline(Deadline deadline) -> void;

    auto setCompleted(bool completed) -> void;

  public:
    [[nodiscard]] virtual auto isForDate(DateTime datetime) const -> bool;

    [[nodiscard]] auto hasDeadline() const -> bool;

  public:
    [[nodiscard]] auto operator==(const SingularTask &other) const
        -> bool = default;

  public:
    struct JSON {
        enum class Error : uint8_t {
            MissingRequiredField,

            FailedToParseTaskData,
            FailedToParseIsCompleted,
        };

        [[nodiscard]] static constexpr auto errorMessage(Error error)
            -> std::string {
            switch (error) {
            case Error::MissingRequiredField:
                return "SingularTask::JSON: missing required field(s)";

            case Error::FailedToParseTaskData:
                return "SingularTask::JSON: failed to parse TaskData";

            case Error::FailedToParseIsCompleted:
                return "SingularTask::JSON: failed to parse isCompleted";

            default:
                std::unreachable();
            }
        }

        static constexpr auto taskDataField{std::string_view{"task_data"}};
        static constexpr auto isCompletedField{
            std::string_view{"is_completed"}};

        static constexpr auto isCompletedTrueValue{std::string_view{"true"}};
        static constexpr auto isCompletedFalseValue{std::string_view{"false"}};

        static constexpr auto fields{
            std::array<std::string_view, 2>{taskDataField, isCompletedField}};

        [[nodiscard]] static auto containsAllFields(const nlohmann::json &json)
            -> bool;

        [[nodiscard]] static auto encode(const SingularTask &singularTask)
            -> nlohmann::json;

        [[nodiscard]] static auto decode(const nlohmann::json &json)
            -> std::expected<SingularTask, JSON::Error>;
    };
};
} // namespace hbt::mods
