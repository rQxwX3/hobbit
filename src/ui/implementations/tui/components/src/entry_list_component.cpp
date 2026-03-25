#include <entry_component.hpp>
#include <entry_list_component.hpp>

namespace hbt::ui::tui {
auto EntryListComponent::setEntries(
    const std::vector<hbt::mods::Entry> &entries) -> void {
    for (const auto &entry : entries) {
        auto child{std::make_shared<EntryComponent>(entry)};

        children_.push_back(child);
        EntryListComponent::Add(child);
    }
}

auto EntryListComponent::OnRender() -> ftxui::Element {
    return ftxui::vbox(children_);
}

} // namespace hbt::ui::tui
