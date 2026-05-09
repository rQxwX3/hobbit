#pragma once

#include <datetime.hpp>
#include <deadline.hpp>

#include <array>
#include <expected>
#include <string>

namespace hbt::mods {
class TaskData {
  private:
    enum class Error : uint8_t {
        EmptyTitle,

        JSONMissingRequiredField,
        JSONEmptyTitle,
    };

  public:
    [[nodiscard]] static constexpr auto errorMessage(Error error)
        -> std::string {
        switch (error) {
        case Error::EmptyTitle:
            return "TaskData: provided title is an empty string";

        case Error::JSONMissingRequiredField:
            return "TaskData: missing required field(s) in JSON";

        case Error::JSONEmptyTitle:
            return "TaskData: provided JSON contains empty title";

        default:
            std::unreachable();
        }
    }

  private:
    static constexpr auto jsonTitleField{std::string_view{"title"}};
    static constexpr auto jsonCompletedField{std::string_view{"completed"}};

    static constexpr auto jsonFields{
        std::array<std::string_view, 2>{jsonTitleField, jsonCompletedField}};

  private:
    [[nodiscard]] auto static validateTitle(const std::string &title)
        -> std::string;

  private:
    std::string title_;

    bool completed_;

  public:
    TaskData(std::string title, bool completed = false);

  public:
    [[nodiscard]] auto getTitle() const -> std::string_view;

    [[nodiscard]] auto getCompleted() const -> bool;

  public:
    auto setTitle(std::string title) -> void;

    auto setCompleted(bool completed) -> void;

  private:
    [[nodiscard]] static auto containsAllJSONFields(const nlohmann::json &json)
        -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::expected<TaskData, Error>;
};
} // namespace hbt::mods
