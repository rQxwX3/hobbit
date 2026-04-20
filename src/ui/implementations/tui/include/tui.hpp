#pragma once

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>

#include <datetime.hpp>
#include <task.hpp>
#include <ui.hpp>

#include <orchestrator_component.hpp>
#include <task_form_component.hpp>
#include <task_list_component.hpp>

#include <vector>

namespace hbt::ui::tui {
class TUI : public UI {
  private:
    std::vector<hbt::mods::TaskData> tasks_;

    ftxui::App screen_;

    std::shared_ptr<OrchestatorComponent> orchestrator_;

    createTaskCallback_t createTaskCallback_;

  private:
    auto switchToScreen(UI::Screen screen) -> void;

  private:
    auto createTaskListComponent() -> ftxui::Component;

    auto createTaskFormComponent() -> ftxui::Component;

    auto setupOrchestrator() -> void;

    // private:
    //   auto static restoreTerminal() -> void;

  public:
    TUI();

  public:
    auto setCreateTaskCallback(const createTaskCallback_t &createTaskCallback)
        -> void override;

    auto setTaskList(std::vector<hbt::mods::TaskData> tasks) -> void override;

    auto populateTaskList(hbt::mods::TaskData task) -> void override;

  public:
    auto start() -> void override;

    auto stop() -> void override;

    auto refresh() -> void override;
};
} // namespace hbt::ui::tui
