#include <entry.hpp>

namespace hbt::mods {
Entry::Entry(std::string_view title, std::vector<Occurence> occurences)
    : occurences_{std::move(occurences)}, title_{title}, isCompleted_{false} {}

auto Entry::setTitle(std::string_view title) -> void { title_ = title; }

auto Entry::toggleIsCompleted() -> void { isCompleted_ = !isCompleted_; }

[[nodiscard]] auto Entry::getTitle() const & -> const std::string & {
    return title_;
}

[[nodiscard]] auto Entry::isCompleted() const -> bool { return isCompleted_; }

[[nodiscard]] auto
Entry::getOccurences() const & -> const std::vector<Occurence> & {
    return occurences_;
}

[[nodiscard]] auto Entry::toJSON() const -> nlohmann::json {
    auto occurencesJSON{nlohmann::json::array()};

    for (const auto &occ : occurences_) {
        occurencesJSON.emplace_back(occ.toJSON());
    }

    return {
        {"occurences", occurencesJSON},
        {"title", title_},
        {"is_completed", isCompleted_},
    };
}

[[nodiscard]] auto Entry::fromJSON(const nlohmann::json &json) -> Entry {
    auto jsonOccurences{json["occurences"]};
    auto occurences{std::vector<Occurence>()};

    for (const auto &jsonOccurence : jsonOccurences) {
        occurences.emplace_back(Occurence::fromJSON(jsonOccurence));
    }

    return Entry{json["title"].get<std::string>(), occurences};
}
} // namespace hbt::mods
