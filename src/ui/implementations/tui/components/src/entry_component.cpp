#include <entry_component.hpp>

namespace hbt::ui::tui {
EntryComponent::EntryComponent(const hbt::mods::Entry &entry)
    : title_{entry.getTitle()}, selected_{false} {}

auto EntryComponent::setSelected(bool selected) -> void {
    selected_ = selected;
}

auto EntryComponent::OnRender() -> ftxui::Element {
    using namespace ftxui;

    auto title{text(title_)};

    if (selected_) {
        title = title | bold | inverted;
    }

    return hbox({title}) | border | flex;
}
} // namespace hbt::ui::tui
