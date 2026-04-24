#pragma once

#include <task_data.hpp>
#include <uuid.hpp>

#include <array>
#include <string>

namespace hbt::mods {
class SingularTask {
  public:
    using deadline_t = TaskData::deadline_t;
    using datetime_t = TaskData::datetime_t;

    using uuid_t = core::uuid::uuid_t;

  private:
    enum class Error : uint8_t {
        JSONMissingRequiredField,
        JSONFailedToParseTaskData,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::JSONMissingRequiredField:
            return "SingularTask: missing required field(s) in JSON";

        case Error::JSONFailedToParseTaskData:
            return "SingularTask: failed to parse TaskData from JSON";
        }
    }

  private:
    static constexpr auto jsonUUIDField{std::string_view{"uuid"}};
    static constexpr auto jsonTaskField{std::string_view{"task"}};

    static constexpr auto jsonFields{
        std::array<std::string_view, 2>{jsonUUIDField, jsonTaskField}};

  private:
    TaskData task_;
    uuid_t uuid_;

  public:
    SingularTask(TaskData task);

    SingularTask(uuid_t uuid, TaskData task);

  public:
    auto setTitle(std::string title) -> void;

    auto setDateTime(datetime_t datetime) -> void;

    auto setDeadline(deadline_t deadline) -> void;

    auto setCompleted(bool completed) -> void;

  public:
    [[nodiscard]] auto getTitle() const -> std::string_view;

    [[nodiscard]] auto getDateTime() const -> datetime_t;

    [[nodiscard]] auto getDeadline() const -> deadline_t;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    [[nodiscard]] virtual auto isForDate(hbt::mods::Date date) const -> bool;

    [[nodiscard]] auto hasDeadline() const -> bool;

  private:
    [[nodiscard]] auto static containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<SingularTask, Error>;
};
} // namespace hbt::mods
