#include "occurrence.hpp"
#include <entry.hpp>

namespace hbt::mods {
Entry::Entry(std::string title, std::vector<Occurrence> occurrences)
    : occurrences_{std::move(occurrences)}, title_{std::move(title)},
      isCompleted_{false} {}

Entry::Entry(std::string title, std::vector<Occurrence> occurrences,
             bool isCompleted)
    : occurrences_{std::move(occurrences)}, title_{std::move(title)},
      isCompleted_{isCompleted} {}

auto Entry::setTitle(std::string title) -> void { title_ = std::move(title); }

auto Entry::setOccurrences(std::vector<Occurrence> occurrences) -> void {
    occurrences_ = std::move(occurrences);
}

auto Entry::setIsCompleted(bool isCompleted) -> void {
    isCompleted_ = isCompleted;
}

[[nodiscard]] auto Entry::getTitle() const & -> const std::string & {
    return title_;
}

[[nodiscard]] auto Entry::isCompleted() const -> bool { return isCompleted_; }

[[nodiscard]] auto
Entry::getOccurrences() const & -> const std::vector<Occurrence> & {
    return occurrences_;
}

[[nodiscard]] auto Entry::toJSON() const & -> nlohmann::json {
    auto occurrencesJSON{nlohmann::json::array()};

    for (const auto &occ : occurrences_) {
        occurrencesJSON.emplace_back(occ.toJSON());
    }

    return {
        {"occurrences", occurrencesJSON},
        {"title", title_},
        {"is_completed", isCompleted_},
    };
}

[[nodiscard]] auto Entry::toJSON() && -> nlohmann::json {
    auto occurrencesJSON{nlohmann::json::array()};

    for (const auto &occ : occurrences_) {
        occurrencesJSON.emplace_back(occ.toJSON());
    }

    return {
        {"occurrences", occurrencesJSON},
        {"title", std::move(title_)},
        {"is_completed", isCompleted_},
    };
}

[[nodiscard]] auto Entry::fromJSON(const nlohmann::json &json) -> Entry {
    if (!json.contains("title") || !json.contains("occurrences") ||
        !json.contains("is_completed")) {
        throw std::runtime_error("Missing required fields");
    }

    auto jsonOccurrences{json["occurrences"]};
    auto occurrences{std::vector<Occurrence>()};

    for (const auto &jsonOccurrence : jsonOccurrences) {
        occurrences.emplace_back(Occurrence::fromJSON(jsonOccurrence));
    }

    return Entry{json["title"].get<std::string>(), occurrences,
                 json["is_completed"].get<bool>()};
}

[[nodiscard]] auto Entry::isForDate(hbt::mods::Date date) -> bool {
    return std::ranges::any_of(occurrences_.begin(), occurrences_.end(),
                               [date](hbt::mods::Occurrence occ) -> bool {
                                   return occ.getWeekday() == date.getWeekday();
                               });
}
} // namespace hbt::mods
