#pragma once

#include <nlohmann/json.hpp>

#include <date.hpp>
#include <occurrence.hpp>

#include <string>
#include <vector>

namespace hbt::mods {
class Entry {
  private:
    std::vector<Occurrence> occurrences_;

    std::string title_;
    bool isCompleted_;

  public:
    Entry(std::string title, std::vector<Occurrence> occurrences);

    Entry(std::string title, std::vector<Occurrence> occurrences,
          bool isCompleted);

  public:
    auto setTitle(std::string title) -> void;

    auto setOccurrences(std::vector<Occurrence> occurrences) -> void;

    auto setIsCompleted(bool isCompleted) -> void;

  public:
    [[nodiscard]] auto getTitle() const & -> const std::string &;

    [[nodiscard]] auto
    getOccurrences() const & -> const std::vector<hbt::mods::Occurrence> &;

    [[nodiscard]] auto isCompleted() const -> bool;

  public:
    [[nodiscard]] auto toJSON() const & -> nlohmann::json;

    [[nodiscard]] auto toJSON() && -> nlohmann::json;

    [[nodiscard]] static auto fromJSON(const nlohmann::json &json) -> Entry;

  public:
    [[nodiscard]] auto isForDate(hbt::mods::Date date) const -> bool;
};
} // namespace hbt::mods
