#include <entry.hpp>

namespace hbt::mods {
Entry::Entry(std::string_view title, std::vector<Occurence> occurences)
    : occurences_{std::move(occurences)}, title_{title}, isCompleted_{false},
      id_{Entry::sIDCounter++} {}

auto Entry::setTitle(std::string_view title) -> void { title_ = title; }

auto Entry::toggleIsCompleted() -> void { isCompleted_ = !isCompleted_; }

[[nodiscard]] auto Entry::getTitle() const -> std::string_view {
    return title_;
}

[[nodiscard]] auto Entry::isCompleted() const -> bool { return isCompleted_; }

[[nodiscard]] auto Entry::getID() const -> id_t { return id_; }

[[nodiscard]] auto Entry::toJSON() const -> nlohmann::json {
    auto occurencesJSON{nlohmann::json::array()};

    for (const auto &occ : occurences_) {
        occurencesJSON.emplace_back(occ.toJSON());
    }

    return {{"occurences", occurencesJSON},
            {"title", title_},
            {"is_completed", isCompleted_},
            {"id", id_}};
}

[[nodiscard]] static auto fromJSON(const nlohmann::json &json) -> Entry {
    auto jsonOccurences{json["occurences"]};
    auto occurences{std::vector<Occurence>()};

    for (const auto &jsonOccurence : jsonOccurences) {
        occurences.emplace_back(Occurence::fromJSON(jsonOccurence));
    }

    return Entry{json["title"].dump(), occurences};
}
} // namespace hbt::mods
