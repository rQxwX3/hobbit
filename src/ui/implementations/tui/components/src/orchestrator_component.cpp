#include <ftxui/component/event.hpp>

#include <orchestrator_component.hpp>

namespace hbt::ui::tui {
OrchestatorComponent::OrchestatorComponent()
    : currentScreen_{UI::Screen::NONE} {}

auto OrchestatorComponent::activateComponent(screen_t screen) -> void {
    if (currentScreen_ == screen) {
        return;
    }

    auto it{cachedComponents_.find(screen)};

    if (it == cachedComponents_.end()) {
        return;
    }

    auto component{it->second.get()};

    currentScreen_ = screen;
    currentComponent_ = component;

    OrchestatorComponent::DetachAllChildren();
    OrchestatorComponent::Add(currentComponent_);
}

auto OrchestatorComponent::registerComponentFactory(UI::Screen screen,
                                                    componentFactory_t factory)
    -> void {
    auto it{cachedComponents_.find(screen)};

    if (it != cachedComponents_.end()) {
        it->second.setFactory(std::move(factory));
        return;
    }

    cachedComponents_.emplace(screen, std::move(factory));
}

auto OrchestatorComponent::invalidateComponent(screen_t screen) -> void {
    auto it{cachedComponents_.find(screen)};

    if (it != cachedComponents_.end()) {
        it->second.invalidate();
    }
}

auto OrchestatorComponent::switchToComponent(screen_t screen) -> void {
    if (currentScreen_ == screen) {
        return;
    }

    activateComponent(screen);
}

auto OrchestatorComponent::OnEvent(ftxui::Event event) -> bool {
    if (event == ftxui::Event::Character('O')) {
        switchToComponent(UI::Screen::TaskList);
        return true;
    }

    if (event == ftxui::Event::Character('P')) {
        switchToComponent(UI::Screen::CreateTask);
        return true;
    }

    return (currentComponent_ && currentComponent_->OnEvent(event));
}

auto OrchestatorComponent::OnRender() -> ftxui::Element {
    if (currentComponent_) {
        return currentComponent_->Render();
    }

    return ftxui::text("No active screen") | ftxui::center;
}
} // namespace hbt::ui::tui
