#include <entry.hpp>

namespace hbt::mods {
Entry::Entry(std::string title, std::vector<Occurence> occurences)
    : occurences_{std::move(occurences)}, title_{std::move(title)},
      isCompleted_{false} {}

auto Entry::setTitle(std::string title) -> void { title_ = std::move(title); }

auto Entry::setOccurences(std::vector<Occurence> occurences) -> void {
    occurences_ = std::move(occurences);
}

auto Entry::toggleIsCompleted() -> void { isCompleted_ = !isCompleted_; }

[[nodiscard]] auto Entry::getTitle() const & -> const std::string & {
    return title_;
}

[[nodiscard]] auto Entry::isCompleted() const -> bool { return isCompleted_; }

[[nodiscard]] auto
Entry::getOccurences() const & -> const std::vector<Occurence> & {
    return occurences_;
}

[[nodiscard]] auto Entry::toJSON() const & -> nlohmann::json {
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

[[nodiscard]] auto Entry::toJSON() && -> nlohmann::json {
    auto occurrencesJSON{nlohmann::json::array()};

    for (const auto &occ : occurences_) {
        occurrencesJSON.emplace_back(occ.toJSON());
    }

    return {
        {"occurrences", occurrencesJSON},
        {"title", std::move(title_)},
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

[[nodiscard]] auto Entry::isForDate(hbt::mods::Date date) -> bool {
    return std::ranges::any_of(occurences_.begin(), occurences_.end(),
                               [date](hbt::mods::Occurence occ) -> bool {
                                   return occ.getWeekday() == date.getWeekday();
                               });
}
} // namespace hbt::mods
