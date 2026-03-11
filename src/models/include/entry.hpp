#pragma once

#include <nlohmann/json.hpp>

#include <chrono>
#include <string>
#include <string_view>
#include <vector>

namespace hbt::mods {
enum class DayPart : char {
    MORNING,
    AFTERNOON,
    EVENING,
    NIGHT,
};

enum class Weekday : char {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
};

struct Occurence {
    Weekday weekday;
    DayPart daypart;

    [[nodiscard]] auto toJSON() const -> nlohmann::json {
        return {{"weekday", weekday}, {"daypart", daypart}};
    }

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json)
        -> Occurence {
        return {.weekday = json["weekday"], .daypart = json["daypart"]};
    }
};

class Entry {
  public:
    using id_t = size_t;

  private:
    inline static id_t sIDCounter{0};

  private:
    std::vector<Occurence> occurences_;

    std::string title_;
    bool isCompleted_;

    id_t id_;

  public:
    Entry(std::string_view title, std::vector<Occurence> occurences);

  public:
    auto setTitle(std::string_view title) -> void;

    auto toggleIsCompleted() -> void;

  public:
    [[nodiscard]] auto getTitle() const -> std::string_view;

    [[nodiscard]] auto isCompleted() const -> bool;

    [[nodiscard]] auto getID() const -> id_t;

  public:
    [[nodiscard]] auto toJSON() const -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json) -> Entry;
};
} // namespace hbt::mods
