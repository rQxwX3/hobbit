#pragma once

#include <ftxui/component/component_base.hpp>

#include <task.hpp>
#include <task_component.hpp>

#include <vector>

namespace hbt::ui::tui {
class TaskListComponent : public ftxui::ComponentBase {
  private:
    using index_t = int;

    static constexpr index_t noSelectionIndex{-1};

  private:
    std::vector<std::shared_ptr<TaskComponent>> children_;

    index_t selectedIndex_{-1};

  private:
    [[nodiscard]] auto getMaxIndex() const -> index_t;

    [[nodiscard]] auto isSafeIndex(index_t index) const -> bool;

    auto updateSelection(index_t newSelectedIndex) -> void;

  public:
    static auto create() -> TaskListComponent;

  public:
    auto setTasks(const std::vector<hbt::mods::TaskData> &tasks) -> void;

  public:
    auto OnRender() -> ftxui::Element override;

    auto OnEvent(ftxui::Event event) -> bool override;
};
} // namespace hbt::ui::tui
