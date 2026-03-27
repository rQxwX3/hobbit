#pragma once

#include <ftxui/component/component_base.hpp>

#include <ui.hpp>

#include <unordered_map>

namespace hbt::ui::tui {
class OrchestatorComponent : public ftxui::ComponentBase {
  public:
    using screen_t = UI::Screen;
    using componentFactory_t = std::function<ftxui::Component()>;

    using screenComponentPair_t = std::pair<UI::Screen, ftxui::Component>;

  private:
    std::unordered_map<screen_t, componentFactory_t> componentFactories_;
    std::unordered_map<screen_t, ftxui::Component> renderedComponents_;

    screen_t currentScreen_;
    ftxui::Component currentComponent_;

  private:
    auto activateComponent(screen_t screen) -> void;

  public:
    explicit OrchestatorComponent();

  public:
    auto registerComponentFactory(screen_t screen,
                                  const componentFactory_t &factory) -> void;

    auto switchToComponent(screen_t screen) -> void;

  public:
    auto OnRender() -> ftxui::Element override;

    auto OnEvent(ftxui::Event event) -> bool override;
};
} // namespace hbt::ui::tui
