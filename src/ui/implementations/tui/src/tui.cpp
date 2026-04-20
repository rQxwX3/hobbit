#include <orchestrator_component.hpp>
#include <task_list_component.hpp>

#include <tui.hpp>

namespace hbt::ui::tui {
auto TUI::createTaskListComponent() -> ftxui::Component {
    auto taskList{ftxui::Make<TaskListComponent>()};

    taskList->setTasks(tasks_);

    return taskList;
}

auto TUI::createTaskFormComponent() -> ftxui::Component {
    if (!createTaskCallback_) {
        throw std::runtime_error("Missing create task callback");
    }

    auto taskForm{ftxui::Make<TaskFormComponent>(
        std::move(createTaskCallback_),
        [this] -> void { switchToScreen(Screen::TaskList); })};

    return taskForm;
}

auto TUI::setupOrchestrator() -> void {
    auto orchestrator{ftxui::Make<OrchestatorComponent>()};

    orchestrator->registerComponentFactory(
        Screen::TaskList,
        [this] -> ftxui::Component { return createTaskListComponent(); });

    orchestrator->registerComponentFactory(
        Screen::CreateTask,
        [this] -> ftxui::Component { return createTaskFormComponent(); });

    orchestrator->switchToComponent(Screen::CreateTask);

    orchestrator_ = orchestrator;
}

auto TUI::setCreateTaskCallback(const createTaskCallback_t &createTaskCallback)
    -> void {
    createTaskCallback_ =
        [this, createTaskCallback](std::string title,
                                   hbt::mods::Interval interval) -> void {
        createTaskCallback(std::move(title), interval);
        orchestrator_->invalidateComponent(Screen::TaskList);
        switchToScreen(Screen::TaskList);
    };
}

auto TUI::start() -> void {
    setupOrchestrator();

    auto quitHandler{
        ftxui::CatchEvent(orchestrator_, [this](ftxui::Event event) {
            if (event == ftxui::Event::Character('q') ||
                event == ftxui::Event::Character('Q')) {
                stop();

                return true;
            }

            return false;
        })};

    screen_.Loop(quitHandler);
}

TUI::TUI() : screen_{ftxui::App::FullscreenAlternateScreen()} {}

auto TUI::setTaskList(std::vector<hbt::mods::TaskData> tasks) -> void {
    tasks_ = std::move(tasks);

    refresh();
}

auto TUI::populateTaskList(hbt::mods::TaskData task) -> void {
    tasks_.push_back(task);
}

auto TUI::switchToScreen(UI::Screen screen) -> void {
    orchestrator_->switchToComponent(screen);
}

auto TUI::stop() -> void { screen_.Exit(); }

auto TUI::refresh() -> void { screen_.PostEvent(ftxui::Event::Custom); }

} // namespace hbt::ui::tui
