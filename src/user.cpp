#include "../include/user.hpp"
#include "../include/entry.hpp"

namespace hbt {
template <typename... Args> auto User::addEntry(Args &&...args) -> void {
    entries_.emplace_back(std::forward<Args>(args)...);
}

auto User::deleteEntry(Entry::id_t id) -> void {
    auto it{std::ranges::find_if(entries_, [id](const Entry &entry) -> bool {
        return entry.getID() == id;
    })};

    if (it != entries_.end()) {
        entries_.erase(it);
    }
}
} // namespace hbt
