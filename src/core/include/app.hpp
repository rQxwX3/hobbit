#pragma once

#include <datetime.hpp>
#include <task_service.hpp>
#include <ui.hpp>

#include <memory>

namespace hbt::core {
class App {
  private:
    std::unique_ptr<hbt::core::TaskService> tasks_;
    std::unique_ptr<hbt::ui::UI> ui_;
    hbt::mods::DateTime date_;

  public:
    App(std::unique_ptr<hbt::core::TaskService> tasks,
        std::unique_ptr<hbt::ui::UI> ui);

  public:
    auto run() -> void;

    auto stop() -> void;
};
} // namespace hbt::core
