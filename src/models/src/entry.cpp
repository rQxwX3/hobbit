#include <entry.hpp>
#include <occurrence.hpp>

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
    auto occurrencesJSON = nlohmann::json::array();

    for (const auto &occ : occurrences_) {
        occurrencesJSON.push_back(occ.toJSON());
    }

    nlohmann::json json = {
        {"occurrences", occurrencesJSON},
        {"title", title_},
        {"is_completed", isCompleted_},
    };

    return json;
}

[[nodiscard]] auto Entry::toJSON() && -> nlohmann::json {
    auto occurrencesJSON = nlohmann::json::array();

    for (auto occ : std::move(occurrences_)) {
        occurrencesJSON.push_back(occ.toJSON());
    }

    nlohmann::json json = {
        {"occurrences", occurrencesJSON},
        {"title", std::move(title_)},
        {"is_completed", isCompleted_},
    };

    return json;
}

[[nodiscard]] auto Entry::fromJSON(const nlohmann::json &json)
    -> std::optional<Entry> {
    if (!json.contains("title") || !json.contains("occurrences") ||
        !json.contains("is_completed")) {
        return std::nullopt;
    }

    auto jsonOccurrences = json["occurrences"];
    auto occurrences{std::vector<Occurrence>()};

    for (const auto &jsonOccurrence : jsonOccurrences) {
        auto occ{Occurrence::fromJSON(jsonOccurrence)};

        if (!occ.has_value()) {
            return std::nullopt;
        }

        occurrences.emplace_back(occ.value());
    }

    return Entry{json["title"].get<std::string>(), occurrences,
                 json["is_completed"].get<bool>()};
}

[[nodiscard]] auto Entry::isForDate(hbt::mods::DateTime date) const -> bool {
    return std::ranges::any_of(occurrences_.begin(), occurrences_.end(),
                               [date](hbt::mods::Occurrence occ) -> bool {
                                   return occ.isForDate(date);
                               });
}
} // namespace hbt::mods
