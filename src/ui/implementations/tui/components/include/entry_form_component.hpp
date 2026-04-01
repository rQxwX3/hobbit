#pragma once

#include <ftxui/component/component_base.hpp>

#include <interval_input_component.hpp>
#include <ui.hpp>

#include <functional>
#include <string>

namespace hbt::ui::tui {
class EntryFormComponent : public ftxui::ComponentBase {
  public:
    using onCancelCallback_t = std::function<void()>;

  private:
    static constexpr std::string_view formTitle{" Create new Entry "};
    static constexpr std::string_view formHelpMenu{
        " Enter: Submit  Esc: Cancel "};

    static constexpr std::string_view titleInputPlaceholder{
        "Enter entry title..."};

    static constexpr std::string_view emptyTitleError{
        "Entry title cannot be empty"};

    static constexpr std::string_view invalidIntervalError{
        "Entry interval is invalid"};

  private:
    std::string title_;
    std::string error_;

    ftxui::Component titleInput_;

    // required to store as shared_ptr to custom class in order to be able to
    // use class methods
    std::shared_ptr<IntervalInputComponent> intervalInput_;
    ftxui::Component container_;

    UI::createEntryCallback_t onSubmit_;
    onCancelCallback_t onCancel_;

  private:
    auto clear() -> void;

  private:
    auto submit() -> void;

    auto cancel() -> void;

  public:
    EntryFormComponent(UI::createEntryCallback_t onSubmit,
                       onCancelCallback_t onCancel);

  public:
    auto OnRender() -> ftxui::Element override;

    auto OnEvent(ftxui::Event event) -> bool override;
};
} // namespace hbt::ui::tui
