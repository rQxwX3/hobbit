#include <entry_service.hpp>

namespace hbt::core {
EntryService::EntryService(
    std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::Entry>>
        repository)
    : repository_{std::move(repository)} {}

auto EntryService::createEntry(std::string title,
                               std::vector<hbt::mods::Occurence> occurences) {
    auto id{repository_->save(
        hbt::mods::Entry{std::move(title), std::move(occurences)})};
}

auto EntryService::deleteEntry(id_t id) { repository_->remove(id); }

auto EntryService::changeEntryTitle(id_t id, std::string title) {
    auto entryOpt{repository_->load(id)};

    if (entryOpt.has_value()) {
        auto entry{std::move(entryOpt.value())};

        entry.setTitle(std::move(title));

        auto id{repository_->save(std::move(entry))};
    }
}

auto EntryService::changeEntryOccurences(
    id_t id, std::vector<hbt::mods::Occurence> occurences) {
    auto entryOpt{repository_->load(id)};

    if (entryOpt.has_value()) {
        auto entry{std::move(entryOpt.value())};

        entry.setOccurences(std::move(occurences));

        auto id{repository_->save(std::move(entry))};
    }
}

auto EntryService::completeEntry(id_t id) {
    auto entryOpt{repository_->load(id)};

    if (entryOpt.has_value()) {
        auto entry{std::move(entryOpt.value())};

        entry.toggleIsCompleted();

        auto id{repository_->save(std::move(entry))};
    }
}
} // namespace hbt::core
