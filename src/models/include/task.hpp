#pragma once

#include <nlohmann/json.hpp>

#include <datetime.hpp>
#include <recurrence_patterns.hpp>

#include <string>
#include <variant>

namespace hbt::mods {
class Task {
  public:
    using deadline_t =
        std::variant<std::monostate, hbt::mods::Interval, hbt::mods::DateTime>;

  private:
    static constexpr auto zeroDeadlineJSON{std::string{"none"}};

  protected:
    std::string title_;

    hbt::mods::DateTime startFrom_;
    deadline_t deadline_;

    bool isCompleted_;

  private:
    [[nodiscard]] virtual auto validateDeadline(deadline_t deadline)
        -> deadline_t;

    [[nodiscard]] virtual auto validateStartFrom(hbt::mods::DateTime startFrom)
        -> hbt::mods::DateTime;

  public:
    Task(std::string title, hbt::mods::DateTime startFrom,
         bool isCompleted = false, deadline_t deadline = std::monostate{});

  public:
    auto setTitle(std::string title) -> void;

    virtual auto setStartFrom(hbt::mods::DateTime startFrom) -> void;

    virtual auto setDeadline(deadline_t deadline) -> void;

    auto setIsCompleted(bool isCompleted) -> void;

  public:
    [[nodiscard]] auto getTitle() const & -> const std::string &;

    [[nodiscard]] auto getStartFrom() const -> hbt::mods::DateTime;

    [[nodiscard]] auto getDeadline() const -> deadline_t;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    [[nodiscard]] virtual auto isForDate(hbt::mods::DateTime datetime) const
        -> bool;

    [[nodiscard]] auto hasDeadline() const -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] auto toJSON() && -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<Task>;

  private:
    [[nodiscard]] auto deadlineToJSON() const -> nlohmann::json;

    [[nodiscard]] static auto deadlineFromJSON(const nlohmann::json &json)
        -> std::optional<deadline_t>;
};
} // namespace hbt::mods
