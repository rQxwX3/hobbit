#pragma once

#include <ftxui/component/component_base.hpp>

#include <entry.hpp>
#include <entry_component.hpp>

#include <memory>
#include <vector>

namespace hbt::ui::tui {
class EntryListComponent : public ftxui::ComponentBase {
  private:
    std::vector<std::shared_ptr<EntryComponent>> children_;
    size_t selectedIndex_{0};

  public:
    auto setEntries(const std::vector<hbt::mods::Entry> &entries) -> void;

  public:
    auto OnRender() -> ftxui::Element override;
};
} // namespace hbt::ui::tui
