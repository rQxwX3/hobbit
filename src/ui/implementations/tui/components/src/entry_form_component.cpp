#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

#include <entry_form_component.hpp>

namespace hbt::ui::tui {
auto EntryFormComponent::clear() -> void {
    title_.clear();
    error_.clear();
}

auto EntryFormComponent::submit() -> void {
    if (title_.empty()) {
        error_ = emptyTitleError;
    } else if (!intervalInput_->isValid()) {
        error_ = invalidIntervalError;
    } else {
        onSubmit_(title_, intervalInput_->getInterval());
        clear();
    }
}

auto EntryFormComponent::cancel() -> void {
    clear();
    onCancel_();
}

EntryFormComponent::EntryFormComponent(UI::createEntryCallback_t onSubmit,
                                       onCancelCallback_t onCancel)
    : onSubmit_{std::move(onSubmit)}, onCancel_{std::move(onCancel)},
      titleInput_{ftxui::Input(&title_, titleInputPlaceholder)},
      intervalInput_{ftxui::Make<IntervalInputComponent>()},
      container_{ftxui::Container::Vertical({titleInput_, intervalInput_})} {
    Add(container_);
}

auto EntryFormComponent::OnRender() -> ftxui::Element {
    using namespace ftxui;

    std::vector<Element> elements{
        text("Title:") | bold,    titleInput_->Render(),    separator(),
        text("Interval:") | bold, intervalInput_->Render(),
    };

    if (!error_.empty()) {
        elements.push_back(separator());
        elements.push_back(text(error_) | color(Color::Red) | bold);
    }

    return vbox(elements) | border;
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

    if (container_->OnEvent(event)) {
        return true;
    }

    return ComponentBase::OnEvent(event);
}
} // namespace hbt::ui::tui
