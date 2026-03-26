#pragma once

#include <ftxui/component/component_base.hpp>

#include <entry.hpp>
#include <entry_component.hpp>

#include <vector>

namespace hbt::ui::tui {
class EntryListComponent : public ftxui::ComponentBase {
  private:
    using index_t = size_t;

  private:
    std::vector<std::shared_ptr<EntryComponent>> children_;

    index_t selectedIndex_{0};

  private:
    auto resetSelection() -> void;

    auto updateSelection(index_t newSelectedIndex) -> void;

  public:
    auto setEntries(const std::vector<hbt::mods::Entry> &entries) -> void;

  public:
    auto OnRender() -> ftxui::Element override;
};
} // namespace hbt::ui::tui
