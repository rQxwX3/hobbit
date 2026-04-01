#include <ftxui/component/component.hpp>
#include <interval.hpp>
#include <interval_input_component.hpp>

namespace hbt::ui::tui {
IntervalInputComponent::IntervalInputComponent()
    : inputField_{ftxui::Input(&inputText_, "e.g., 2y 3mo")} {
    Add(inputField_);
}

[[nodiscard]] auto IntervalInputComponent::isTimeToParse() const -> bool {
    auto now{std::chrono::steady_clock::now()};
    auto elapsed{std::chrono::duration_cast<std::chrono::milliseconds>(
        now - lastKeystroke_)};

    return elapsed > debounce;
}

auto IntervalInputComponent::triggerParse() -> void {
    isLoading_ = true;

    auto result{hbt::mods::Interval::fromNaturalLanguage(inputText_)};

    if (result.has_value()) {
        previewText_ = "result is smth";
        hasError_ = false;
    } else {
        previewText_ = "invalid";
        hasError_ = true;
    }
}

auto IntervalInputComponent::OnEvent(ftxui::Event event) -> bool {
    bool handled = inputField_->OnEvent(event);

    if (handled && (event.is_character() || event == ftxui::Event::Backspace ||
                    event == ftxui::Event::Delete)) {
        lastKeystroke_ = std::chrono::steady_clock::now();
        isLoading_ = true;
        previewText_ = "Loading...";
    }

    if (isLoading_) {
        triggerParse();
        isLoading_ = false;
    }

    return handled;
}

auto IntervalInputComponent::OnRender() -> ftxui::Element {
    using namespace ftxui;

    auto previewStyle = hasError_ ? color(Color::Red) : color(Color::Green);
    auto previewElement = isLoading_ ? text(previewText_) | color(Color::Yellow)
                                     : text(previewText_) | previewStyle;

    return vbox({inputField_->Render(), separator(),
                 hbox({text("Preview: "), previewElement}),
                 text("Pause typing to parse") | dim}) |
           border;
}
} // namespace hbt::ui::tui
