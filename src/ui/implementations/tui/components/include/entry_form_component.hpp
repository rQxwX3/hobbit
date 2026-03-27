#pragma once

#include <ftxui/component/component_base.hpp>

#include <functional>
#include <string>

namespace hbt::ui::tui {
class EntryFormComponent : public ftxui::ComponentBase {
  public:
    using onSubmitCallback_t = std::function<void(const std::string &)>;
    using onCancelCallback_t = std::function<void()>;

  private:
    static constexpr std::string_view formTitle{" Create new Entry "};
    static constexpr std::string_view formHelpMenu{
        " Enter: Submit  Esc: Cancel  Tab: Navigate "};

    static constexpr std::string_view submitButtonText{"Create"};
    static constexpr std::string_view cancelButtonText{"Cancel"};

    static constexpr std::string_view titleInputPlaceholder{
        "Enter entry title..."};

    static constexpr std::string_view emptyTitleError{
        "Entry title cannot be empty"};

  private:
    std::string title_;
    std::string error_;

    ftxui::Component titleInput_;
    ftxui::Component submitButton_;
    ftxui::Component cancelButton_;

    onSubmitCallback_t onSubmit_;

  private:
    auto clear() -> void;

    auto createElements() -> std::vector<ftxui::Element>;

  private:
    auto submit() -> void;

    auto cancel() -> void;

  public:
    EntryFormComponent(onSubmitCallback_t onSubmit);

  public:
    auto OnRender() -> ftxui::Element override;

    auto OnEvent(ftxui::Event event) -> bool override;
};
} // namespace hbt::ui::tui
