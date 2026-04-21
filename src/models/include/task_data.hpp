#pragma once

#include <datetime.hpp>

#include <optional>
#include <string>
#include <variant>

namespace hbt::mods {
struct TaskData {
  public:
    using deadline_t =
        std::variant<std::monostate, hbt::mods::Interval, hbt::mods::DateTime>;
    using start_t = hbt::mods::DateTime;

  public:
    std::string title;

    start_t start;
    deadline_t deadline;

    bool isCompleted;

  public:
    TaskData(std::string title, start_t start, bool isCompleted = false,
             deadline_t deadline = std::monostate{});

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<TaskData>;

  private:
    [[nodiscard]] auto deadlineToJSON() const -> nlohmann::json;

    [[nodiscard]] static auto deadlineFromJSON(const nlohmann::json &json)
        -> std::optional<deadline_t>;
};
} // namespace hbt::mods
