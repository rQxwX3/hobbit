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

  private:
    enum class Error : uint8_t {
        JSONMissingRequiredField,

        JSONFailedToParseDateTime,
        JSONFailedToParseDeadline,
    };

  private:
    static constexpr auto jsonTitleField{std::string_view{"title"}};
    static constexpr auto jsonDateTimeField{std::string_view{"datetime"}};
    static constexpr auto jsonDeadlineField{std::string_view{"deadline"}};
    static constexpr auto jsonCompletedField{std::string_view{"completed"}};

    static constexpr auto jsonFields{
        std::array<std::string_view, 4>{jsonTitleField, jsonDateTimeField,
                                        jsonDeadlineField, jsonCompletedField}};

    static constexpr auto jsonNullDeadlineValue{std::string_view{"none"}};

  public:
    std::string title;

    DateTime datetime;
    deadline_t deadline;

    bool completed;

  public:
    TaskData(std::string title, DateTime datetime, bool completed = false,
             deadline_t deadline = std::nullopt);

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
