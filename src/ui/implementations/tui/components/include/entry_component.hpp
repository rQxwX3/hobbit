#pragma once

#include <ftxui/component/component.hpp>

#include <entry.hpp>

#include <string>

namespace hbt::ui::tui {
class EntryComponent : public ftxui::ComponentBase {
  private:
    std::string title_;
    bool selected_;

  public:
    explicit EntryComponent(const hbt::mods::Entry &entry);

  public:
    auto setSelected(bool selected) -> void;

  public:
    auto OnRender() -> ftxui::Element override;
};
} // namespace hbt::ui::tui
