#pragma once

#include <ftxui/component/component.hpp>

#include <string>

namespace hbt::ui::tui {
class EntryComponent : public ftxui::ComponentBase {
  private:
    std::string title_;

  public:
    explicit EntryComponent(std::string title);

  public:
    auto OnRender() -> ftxui::Element override;
};
} // namespace hbt::ui::tui
