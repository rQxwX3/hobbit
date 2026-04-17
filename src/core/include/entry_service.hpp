#pragma once

#include <datetime.hpp>
#include <entry.hpp>
#include <repository.hpp>

#include <memory>

namespace hbt::core {
class EntryService {
  private:
    using id_t = size_t;

  private:
    std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::Entry>>
        repository_;

  public:
    explicit EntryService(
        std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::Entry>>
            repository);

  public:
    auto createEntry(std::string title,
                     std::vector<hbt::mods::Occurrence> occurrences) -> id_t;

    auto createEntry(const hbt::mods::Entry &entry) -> id_t;

    auto deleteEntry(id_t id) -> void;

  public:
    auto changeEntryTitle(id_t id, std::string title) -> void;

    auto changeEntryOccurrences(id_t id,
                                std::vector<hbt::mods::Occurrence> occurrences)
        -> void;

    auto completeEntry(id_t id) -> void;

    auto uncompleteEntry(id_t id) -> void;

  public:
    [[nodiscard]] auto getEntriesForDate(const mods::DateTime &date) const
        -> std::vector<hbt::mods::Entry>;

    [[nodiscard]] auto getCount() const -> size_t;
};
} // namespace hbt::core
