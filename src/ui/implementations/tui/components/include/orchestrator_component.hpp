#pragma once

#include <ftxui/component/component_base.hpp>

#include <ui.hpp>

#include <unordered_map>

namespace hbt::ui::tui {
class OrchestatorComponent : public ftxui::ComponentBase {
  public:
    using screen_t = UI::Screen;
    using componentFactory_t = std::function<ftxui::Component()>;

    class CachedComponent {
      private:
        componentFactory_t factory_;
        ftxui::Component cached_;

      public:
        CachedComponent(componentFactory_t factory)
            : factory_{std::move(factory)} {}

        auto get() -> ftxui::Component {
            if (!cached_) {
                assert(factory_);
                cached_ = factory_();
            }

            return cached_;
        }

        auto setFactory(componentFactory_t factory) {
            factory_ = std::move(factory);
        }

        auto setCached(ftxui::Component component) {
            cached_ = std::move(component);
        }

        auto invalidate() { cached_ = nullptr; }
    };

  private:
    std::unordered_map<screen_t, CachedComponent> cachedComponents_;

    screen_t currentScreen_;
    ftxui::Component currentComponent_;

  private:
    auto activateComponent(screen_t screen) -> void;

  public:
    explicit OrchestatorComponent();

  public:
    auto registerComponentFactory(screen_t screen, componentFactory_t factory)
        -> void;

    auto invalidateComponent(screen_t screen) -> void;

    auto switchToComponent(screen_t screen) -> void;

  public:
    auto OnRender() -> ftxui::Element override;

    auto OnEvent(ftxui::Event event) -> bool override;
};
} // namespace hbt::ui::tui
