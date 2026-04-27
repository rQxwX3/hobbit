#pragma once

#include <datetime.hpp>
#include <task_manager.hpp>
#include <ui.hpp>

#include <memory>

namespace hbt::core {
class App {
  public:
    using tasks_t = std::unique_ptr<hbt::core::TaskManager>;
    using ui_t = std::unique_ptr<hbt::ui::UI>;

  private:
    tasks_t tasks_;
    ui_t ui_;

  public:
    App(tasks_t tasks, ui_t ui);

  public:
    auto run() -> void;

    auto stop() -> void;
};
} // namespace hbt::core
