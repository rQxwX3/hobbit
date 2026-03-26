#pragma once

#include <ftxui/component/component_base.hpp>

#include <entry.hpp>
#include <entry_component.hpp>

#include <vector>

namespace hbt::ui::tui {
class EntryListComponent : public ftxui::ComponentBase {
  private:
    using index_t = int;

    static constexpr index_t noSelectionIndex{-1};

  private:
    std::vector<std::shared_ptr<EntryComponent>> children_;

    index_t selectedIndex_{-1};

  private:
    [[nodiscard]] auto getMaxIndex() const -> index_t;

    [[nodiscard]] auto isSafeIndex(index_t index) const -> bool;

    auto updateSelection(index_t newSelectedIndex) -> void;

  public:
    auto setEntries(const std::vector<hbt::mods::Entry> &entries) -> void;

  public:
    auto OnRender() -> ftxui::Element override;

    auto OnEvent(ftxui::Event event) -> bool override;
};
} // namespace hbt::ui::tui
