#pragma once

#include <nlohmann/json.hpp>

#include <datetime.hpp>
#include <recurrence_patterns.hpp>

#include <string>

namespace hbt::mods {
class Task {
  public:
    using deadline_t = std::optional<hbt::mods::DateTime>;

  private:
    static constexpr auto zeroDeadlineJSON{std::string{"none"}};

  protected:
    std::string title_;

    hbt::mods::DateTime startFrom_;
    deadline_t deadline_;

    bool isCompleted_;

  public:
    Task(std::string title, hbt::mods::DateTime startFrom,
         deadline_t deadline = std::nullopt, bool isCompleted = false);

  public:
    auto setTitle(std::string title) -> void;

    auto setStartFrom(hbt::mods::DateTime startFrom) -> void;

    auto setDeadline(deadline_t deadline) -> void;

    auto setIsCompleted(bool isCompleted) -> void;

  public:
    [[nodiscard]] auto getTitle() const & -> const std::string &;

    [[nodiscard]] auto getStartFrom() const -> hbt::mods::DateTime;

    [[nodiscard]] auto getDeadline() const -> deadline_t;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] auto toJSON() && -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<Task>;
};
} // namespace hbt::mods
