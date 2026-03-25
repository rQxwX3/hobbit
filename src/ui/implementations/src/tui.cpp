#include <tui.hpp>

#include <iostream>

namespace hbt::ui {
auto TUI::createMainComponent() -> ftxui::Component {
    auto component = ftxui::Renderer([&] -> ftxui::Element {
        return ftxui::text("Habit Tracker") | ftxui::center;
    });

    component =
        ftxui::CatchEvent(component, [&](const ftxui::Event &event) -> bool {
            if (event == ftxui::Event::Character('q')) {
                stop();
                return true;
            }

            return false;
        });

    return component;
}

TUI::TUI() : screen_{ftxui::App::FullscreenAlternateScreen()} {}

TUI::~TUI() = default;

auto TUI::start() -> void {
    auto component{createMainComponent()};

    screen_.Loop(component);
}

auto TUI::stop() -> void { screen_.Exit(); }

auto TUI::refresh() -> void { screen_.PostEvent(ftxui::Event::Custom); }
} // namespace hbt::ui
