#include <ftxui/component/event.hpp>

#include <orchestrator_component.hpp>

namespace hbt::ui::tui {
OrchestatorComponent::OrchestatorComponent()
    : currentScreen_{UI::Screen::NONE} {}

auto OrchestatorComponent::activateComponent(screen_t screen) -> void {
    if (!renderedComponents_.contains(screen)) {
        auto it{componentFactories_.find(screen)};

        if (it == componentFactories_.end()) {
            return;
        }

        renderedComponents_[screen] = it->second();
    }

    currentScreen_ = screen;
    currentComponent_ = renderedComponents_[screen];

    OrchestatorComponent::DetachAllChildren();
    OrchestatorComponent::Add(currentComponent_);
}

auto OrchestatorComponent::registerComponentFactory(
    UI::Screen screen, const componentFactory_t &factory) -> void {
    componentFactories_[screen] = factory;
}

auto OrchestatorComponent::switchToComponent(screen_t screen) -> void {
    if (currentScreen_ == screen) {
        return;
    }

    activateComponent(screen);
}

auto OrchestatorComponent::OnEvent(ftxui::Event event) -> bool {
    if (event == ftxui::Event::Character('1')) {
        switchToComponent(UI::Screen::EntryList);
    }

    if (event == ftxui::Event::Character('2')) {
        switchToComponent(UI::Screen::CreateEntry);
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
