#include <algorithm>

#include <date.hpp>
#include <entry_service.hpp>

namespace hbt::core {
EntryService::EntryService(
    std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::Entry>>
        repository)
    : repository_{std::move(repository)} {}

auto EntryService::createEntry(std::string title,
                               std::vector<hbt::mods::Occurence> occurences)
    -> id_t {
    return repository_->save(
        hbt::mods::Entry{std::move(title), std::move(occurences)});
}

auto EntryService::deleteEntry(id_t id) -> void { repository_->remove(id); }

auto EntryService::changeEntryTitle(id_t id, std::string title) -> void {
    auto entryOpt{repository_->load(id)};

    if (entryOpt.has_value()) {
        auto entry{std::move(entryOpt.value())};

        entry.setTitle(std::move(title));

        repository_->save(std::move(entry));
    }
}

auto EntryService::changeEntryOccurences(
    id_t id, std::vector<hbt::mods::Occurence> occurences) -> void {
    auto entryOpt{repository_->load(id)};

    if (entryOpt.has_value()) {
        auto entry{std::move(entryOpt.value())};

        entry.setOccurences(std::move(occurences));

        repository_->save(std::move(entry));
    }
}

auto EntryService::completeEntry(id_t id) -> void {
    auto entryOpt{repository_->load(id)};

    if (entryOpt.has_value()) {
        auto entry{std::move(entryOpt.value())};

        entry.toggleIsCompleted();

        repository_->save(std::move(entry));
    }
}

[[nodiscard]] auto EntryService::getEntriesForDate() const
    -> std::vector<hbt::mods::Entry> {
    auto entriesForDate{std::vector<hbt::mods::Entry>()};

    std::ranges::copy_if(repository_->getAll(), entriesForDate.begin(),
                         [](hbt::mods::Entry entry) -> bool {
                             return entry.isForDate(mods::Date::today());
                         });

    return entriesForDate;
}
} // namespace hbt::core
