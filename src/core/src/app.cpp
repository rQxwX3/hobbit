#include <app.hpp>

namespace hbt::core {
App::App(std::unique_ptr<hbt::core::TaskService> tasks,
         std::unique_ptr<hbt::ui::UI> ui)
    : tasks_{std::move(tasks)}, ui_{std::move(ui)},
      date_{hbt::mods::DateTime::today()} {

    ui_->setCreateTaskCallback([this](std::string title,
                                      hbt::mods::Interval interval) -> void {
        auto task{hbt::mods::Task{
            std::move(title),
            {hbt::mods::Occurrence{hbt::mods::DateTime::today(), interval}}}};

        tasks_->createTask(task);
        ui_->populateTaskList(task);
    });

    ui_->setTaskList(tasks_->getTasksForDate(date_));
};

auto App::run() -> void { ui_->start(); }

auto App::stop() -> void { ui_->stop(); }
} // namespace hbt::core
