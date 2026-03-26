#include <entry_component.hpp>
#include <entry_list_component.hpp>

namespace hbt::ui::tui {
auto EntryListComponent::updateSelection(index_t newSelectedIndex) -> void {
    children_[selectedIndex_]->setSelected(false);

    selectedIndex_ = newSelectedIndex;

    children_[selectedIndex_]->setSelected(true);
}

auto EntryListComponent::resetSelection() -> void { selectedIndex_ = -1; }

auto EntryListComponent::setEntries(
    const std::vector<hbt::mods::Entry> &entries) -> void {
    children_.clear();

    for (const auto &entry : entries) {
        auto child{std::make_shared<EntryComponent>(entry)};

        children_.push_back(child);
        EntryListComponent::Add(child);
    }

    updateSelection(0);
}

auto EntryListComponent::OnRender() -> ftxui::Element {
    using namespace ftxui;

    std::vector<Element> elements;
    elements.reserve(children_.size());

    for (auto &child : children_) {
        elements.push_back(child->Render());
    }

    auto width{Terminal::Size().dimx / 3};
    auto list{vbox(elements) | size(WIDTH, EQUAL, width)};

    return center(list) | flex;
}

} // namespace hbt::ui::tui
