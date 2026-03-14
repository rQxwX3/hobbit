#include <entry.hpp>
#include <repository.hpp>

#include <memory>

namespace hbt::core {
class EntryService {
  private:
    using id_t = size_t;

  private:
    std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::Entry>>
        _repository;

  public:
    explicit EntryService(
        std::unique_ptr<hbt::repo::MultiItemRepository<hbt::mods::Entry>>
            repository);

  public:
    auto createEntry(std::string title,
                     std::vector<hbt::mods::Occurence> occurences);

    auto deleteEntry(id_t id);

  public:
    auto changeEntryTitle(const std::string &title);

    auto
    changeEntryOccurences(const std::vector<hbt::mods::Occurence> &occurences);

    auto completeEntry(id_t id);
};
} // namespace hbt::core
