#pragma once

#include <ftxui/component/component.hpp>

#include <task.hpp>

#include <string>

namespace hbt::ui::tui {
class TaskComponent : public ftxui::ComponentBase {
  private:
    std::string title_;
    bool selected_;

  public:
    explicit TaskComponent(const hbt::mods::TaskData &task);

  public:
    auto setSelected(bool selected) -> void;

  public:
    auto OnRender() -> ftxui::Element override;
};
} // namespace hbt::ui::tui
