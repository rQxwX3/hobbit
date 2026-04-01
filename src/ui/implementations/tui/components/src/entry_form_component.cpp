#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

#include <entry_form_component.hpp>

namespace hbt::ui::tui {
auto EntryFormComponent::submit() -> void {
    if (title_.empty()) {
        error_ = emptyTitleError;
    } else {
        onSubmit_(title_);
        clear();
    }
}

auto EntryFormComponent::cancel() -> void {
    clear();
    onCancel_();
}

EntryFormComponent::EntryFormComponent(onSubmitCallback_t onSubmit,
                                       onCancelCallback_t onCancel)
    : onSubmit_{std::move(onSubmit)}, onCancel_{std::move(onCancel)},
      titleInput_{ftxui::Input(&title_, titleInputPlaceholder)},
      intervalInput_{ftxui::Make<IntervalInputComponent>()} {
    Add(titleInput_);
    Add(intervalInput_);
}

auto EntryFormComponent::clear() -> void {
    title_.clear();
    error_.clear();
}

auto EntryFormComponent::OnRender() -> ftxui::Element {
    using namespace ftxui;

    using namespace ftxui;

    return vbox({// text("Title:"),
                 // titleInput_->Render(),
                 text("Interval Input:"), intervalInput_->Render()}) |
           border;
}

auto EntryFormComponent::OnEvent(ftxui::Event event) -> bool {
    using namespace ftxui;

    if (event == Event::Escape) {
        cancel();

        return true;
    }

    if (event == Event::Return) {
        submit();

        return true;
    }

    // if (titleInput_->OnEvent(event)) {
    //     return true;
    // }

    if (intervalInput_->OnEvent(event)) {
        return true;
    }

    return ComponentBase::OnEvent(event);
}
} // namespace hbt::ui::tui
