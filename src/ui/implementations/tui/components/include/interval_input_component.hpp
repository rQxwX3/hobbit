#pragma once

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>

#include <chrono>

namespace hbt::ui::tui {
class IntervalInputComponent : public ftxui::ComponentBase {
  private:
    std::string inputText_;
    std::string previewText_;

    bool isLoading_{false};
    bool hasError_{false};

    std::chrono::steady_clock::time_point lastKeystroke_;

    ftxui::Component inputField_;

  private:
    static constexpr std::chrono::milliseconds debounce{1000};

  public:
    IntervalInputComponent();

  public:
    [[nodiscard]] auto isTimeToParse() const -> bool;

    auto triggerParse() -> void;

  public:
    auto OnEvent(ftxui::Event event) -> bool override;

    auto OnRender() -> ftxui::Element override;
};
} // namespace hbt::ui::tui
