#include <entry_component.hpp>
#include <entry_list_component.hpp>

#include <ftxui/component/event.hpp>

namespace hbt::ui::tui {
[[nodiscard]] auto EntryListComponent::getMaxIndex() const -> index_t {
    auto childrenSize{static_cast<index_t>(children_.size())};

    return (childrenSize == 0) ? noSelectionIndex : childrenSize;
}

[[nodiscard]] auto EntryListComponent::isSafeIndex(index_t index) const
    -> bool {
    if (index < 0) {
        return false;
    }

    if (auto maxIndex{getMaxIndex()}; maxIndex != noSelectionIndex) {
        return index < maxIndex;
    }

    return false;
}

auto EntryListComponent::updateSelection(index_t newSelectedIndex) -> void {
    if (isSafeIndex(selectedIndex_)) {
        children_[selectedIndex_]->setSelected(false);
    }

    selectedIndex_ = newSelectedIndex;

    if (isSafeIndex(selectedIndex_)) {
        children_[selectedIndex_]->setSelected(true);
    }
}

auto EntryListComponent::setEntries(
    const std::vector<hbt::mods::Entry> &entries) -> void {
    children_.clear();

    for (const auto &entry : entries) {
        auto child{std::make_shared<EntryComponent>(entry)};

        children_.push_back(child);
        EntryListComponent::Add(child);
    }

    updateSelection(entries.empty() ? noSelectionIndex : 0);
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

auto EntryListComponent::OnEvent(ftxui::Event event) -> bool {
    using namespace ftxui;

    if (event == Event::Character('k') || event == Event::ArrowUp) {
        if (auto attemptIndex{selectedIndex_ - 1}; isSafeIndex(attemptIndex)) {
            updateSelection(attemptIndex);
        }

        return true;
    }

    if (event == Event::Character('j') || event == Event::ArrowDown) {
        if (auto attemptIndex{selectedIndex_ + 1}; isSafeIndex(attemptIndex)) {
            updateSelection(attemptIndex);
        }

        return true;
    }

    return false;
}
} // namespace hbt::ui::tui
