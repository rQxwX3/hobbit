#include <entry_component.hpp>

namespace hbt::ui::tui {
EntryComponent::EntryComponent(std::string title) : title_{std::move(title)} {}

auto EntryComponent::OnRender() -> ftxui::Element {
    return ftxui::text(title_);
}
} // namespace hbt::ui::tui
