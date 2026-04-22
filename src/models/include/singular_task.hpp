#pragma once

#include <task_data.hpp>

#include <string>

namespace hbt::mods {
class SingularTask {
  public:
    using deadline_t = TaskData::deadline_t;
    using start_t = TaskData::start_t;
    using uuid_t = std::string;

  private:
    static constexpr auto zeroDeadlineJSON{std::string_view{"none"}};

  private:
    static constexpr auto invalidDeadlineError{
        std::string_view{"Tasks can't have deadlines before their date"}};

    static constexpr auto invalidStartFromError{
        std::string_view{"Tasks can't happen after their deadline"}};

  private:
    TaskData task_;
    uuid_t uuid_;

  public:
    SingularTask(const TaskData &task);

    SingularTask(uuid_t uuid, const TaskData &task);

  private:
    [[nodiscard]] auto validateTaskData(const TaskData &task) const -> TaskData;

    auto validateDeadline(deadline_t deadline) const -> deadline_t;

    auto validateStart(start_t start) const -> start_t;

  public:
    auto setTitle(std::string title) -> void;

    auto setStart(start_t start) -> void;

    auto setDeadline(deadline_t deadline) -> void;

    auto setIsCompleted(bool isCompleted) -> void;

  public:
    [[nodiscard]] auto getTitle() const & -> const std::string &;

    [[nodiscard]] auto getStart() const -> start_t;

    [[nodiscard]] auto getDeadline() const -> deadline_t;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    [[nodiscard]] virtual auto isForDate(hbt::mods::Date date) const -> bool;

    [[nodiscard]] auto hasDeadline() const -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> std::optional<SingularTask>;
};
} // namespace hbt::mods
