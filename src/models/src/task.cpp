#include <task.hpp>

namespace hbt::mods {
[[nodiscard]] auto Task::validateDeadline(deadline_t deadline) -> deadline_t {
    if (!std::holds_alternative<hbt::mods::DateTime>(deadline)) {
        return deadline;
    }

    if (auto datetime{std::get<hbt::mods::DateTime>(deadline)};
        datetime < startFrom_) {
        throw std::invalid_argument(std::string{invalidDeadlineError});
    }

    return deadline;
}

[[nodiscard]] auto Task::validateStartFrom(hbt::mods::DateTime startFrom)
    -> hbt::mods::DateTime {
    if (std::holds_alternative<hbt::mods::DateTime>(deadline_) &&
        startFrom > std::get<hbt::mods::DateTime>(deadline_)) {
        throw std::invalid_argument(std::string{invalidStartFromError});
    }

    return startFrom;
}

Task::Task(std::string title, hbt::mods::DateTime startFrom, bool isCompleted,
           deadline_t deadline)
    : title_{std::move(title)}, startFrom_{startFrom},
      isCompleted_{isCompleted},
      deadline_{validateDeadline(std::move(deadline))} {}

auto Task::setTitle(std::string title) -> void { title_ = std::move(title); }

auto Task::setStartFrom(hbt::mods::DateTime startFrom) -> void {
    startFrom_ = validateStartFrom(startFrom);
}

auto Task::setDeadline(deadline_t deadline) -> void {
    deadline_ = validateDeadline(std::move(deadline));
}

auto Task::setIsCompleted(bool isCompleted) -> void {
    isCompleted_ = isCompleted;
}

[[nodiscard]] auto Task::getTitle() const & -> const std::string & {
    return title_;
}

[[nodiscard]] auto Task::getStartFrom() const -> hbt::mods::DateTime {
    return startFrom_;
}

[[nodiscard]] auto Task::getDeadline() const -> deadline_t { return deadline_; }

[[nodiscard]] auto Task::isCompleted() const -> bool { return isCompleted_; }

[[nodiscard]] auto Task::isForDate(hbt::mods::DateTime datetime) const -> bool {
    return DateTime::equalDates(startFrom_, datetime);
}

[[nodiscard]] auto Task::hasDeadline() const -> bool {
    return !std::holds_alternative<std::monostate>(deadline_);
}

[[nodiscard]] auto Task::toJSON() const & -> nlohmann::json {
    nlohmann::json json = {
        {"title", title_},
        {"datetime", startFrom_.toISO8601String()},
        {"deadline", deadlineToJSON()},
        {"is_completed", isCompleted_},
    };

    return json;
}

[[nodiscard]] auto Task::toJSON() && -> nlohmann::json {
    nlohmann::json json = {
        {"title", std::move(title_)},
        {"datetime", startFrom_.toISO8601String()},
        {"deadline", deadlineToJSON()},
        {"is_completed", isCompleted_},
    };

    return json;
}

[[nodiscard]] auto Task::fromJSON(const nlohmann::json &json)
    -> std::optional<Task> {
    if (!json.contains("title") || !json.contains("is_completed") ||
        !json.contains("datetime") || !json.contains("deadline")) {
        return std::nullopt;
    }

    auto dateTimeFromISO8601{hbt::mods::DateTime::fromISO8601String(
        json["datetime"].get<std::string>())};
    if (!dateTimeFromISO8601.has_value()) {
        return std::nullopt;
    }

    auto deadlineFromJSON{Task::deadlineFromJSON(json["deadline"])};
    if (!deadlineFromJSON.has_value()) {
        return std::nullopt;
    }

    return Task{json["title"].get<std::string>(), dateTimeFromISO8601.value(),
                json["is_completed"].get<bool>(), deadlineFromJSON.value()};
}

[[nodiscard]] auto Task::deadlineToJSON() const -> nlohmann::json {
    if (std::holds_alternative<std::monostate>(deadline_)) {
        return {{"type", "none"}};
    }

    if (std::holds_alternative<hbt::mods::Interval>(deadline_)) {
        auto intervalJSON{std::get<hbt::mods::Interval>(deadline_).toJSON()};

        return {{"type", "interval"}, {"interval", intervalJSON}};
    }

    auto datetimeISO8601{
        std::get<hbt::mods::DateTime>(deadline_).toISO8601String()};

    return {{"type", "datetime"}, {"datetime", datetimeISO8601}};
}

[[nodiscard]] auto Task::deadlineFromJSON(const nlohmann::json &json)
    -> std::optional<deadline_t> {
    if (!json.contains("type")) {
        return std::nullopt;
    }

    if (json["type"] == "none") {
        return std::monostate{};
    }

    if (json["type"] == "interval") {
        if (!json.contains("interval")) {
            return std::nullopt;
        }

        return hbt::mods::Interval::fromJSON(json["interval"]);
    }

    if (json["type"] == "datetime") {
        if (!json.contains("datetime")) {
            return std::nullopt;
        }

        return hbt::mods::DateTime::fromISO8601String(json["datetime"]);
    }

    return std::nullopt;
}
} // namespace hbt::mods
