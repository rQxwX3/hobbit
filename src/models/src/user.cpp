#include "../include/user.hpp"
#include "../include/entry.hpp"

#include <algorithm>

namespace hbt {
auto User::deleteEntry(Entry::id_t id) -> void {
    auto it{std::ranges::find_if(entries_, [id](const Entry &entry) -> bool {
        return entry.getID() == id;
    })};

    if (it != entries_.end()) {
        entries_.erase(it);
    }
}

[[nodiscard]] auto User::getEntries() & -> const std::vector<Entry> & {
    return entries_;
}
} // namespace hbt
