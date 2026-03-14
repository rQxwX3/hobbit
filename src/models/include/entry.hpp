#pragma once

#include <nlohmann/json.hpp>

#include <date.hpp>
#include <occurence.hpp>

#include <string>
#include <vector>

namespace hbt::mods {
class Entry {
  private:
    std::vector<Occurence> occurences_;

    std::string title_;
    bool isCompleted_;

  public:
    Entry(std::string title, std::vector<Occurence> occurences);

  public:
    auto setTitle(std::string title) -> void;

    auto setOccurences(std::vector<Occurence> occurences) -> void;

    auto toggleIsCompleted() -> void;

  public:
    [[nodiscard]] auto getTitle() const & -> const std::string &;

    [[nodiscard]] auto
    getOccurences() const & -> const std::vector<Occurence> &;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] auto toJSON() && -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json) -> Entry;

  public:
    [[nodiscard]] auto isForDate(hbt::mods::Date date) -> bool;
};
} // namespace hbt::mods
