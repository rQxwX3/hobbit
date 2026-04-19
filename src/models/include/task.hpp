#pragma once

#include <nlohmann/json.hpp>

#include <datetime.hpp>
#include <recurrence_patterns.hpp>

#include <string>

namespace hbt::mods {
class Task {
  private:
    std::string title_;
    bool isCompleted_;

  public:
    Task(std::string title);

    Task(std::string title, bool isCompleted);

  public:
    auto setTitle(std::string title) -> void;

    auto setIsCompleted(bool isCompleted) -> void;

  public:
    [[nodiscard]] auto getTitle() const & -> const std::string &;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] auto toJSON() && -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<Task>;
};
} // namespace hbt::mods
