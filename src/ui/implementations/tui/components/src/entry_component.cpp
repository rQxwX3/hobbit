#include <entry_component.hpp>

namespace hbt::ui::tui {
EntryComponent::EntryComponent(const hbt::mods::Entry &entry)
    : title_{entry.getTitle()} {}

auto EntryComponent::OnRender() -> ftxui::Element {
    return ftxui::text(title_);
}
} // namespace hbt::ui::tui
