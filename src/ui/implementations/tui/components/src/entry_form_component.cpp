#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

#include <entry_form_component.hpp>

namespace hbt::ui::tui {
EntryFormComponent::EntryFormComponent(onSubmitCallback_t onSubmit)
    : onSubmit_{std::move(onSubmit)},

      titleInput_{ftxui::Input(&title_, titleInputPlaceholder)},

      submitButton_{ftxui::Button(submitButtonText,
                                  [this]() -> void {
                                      if (title_.empty()) {
                                          error_ = emptyTitleError;
                                      } else {
                                          onSubmit_(title_);
                                          clear();
                                      }
                                  })},

      cancelButton_{ftxui::Button(cancelButtonText, [this]() -> void {
          onCancel_();
          clear();
      })} {
    Add(titleInput_);
    Add(submitButton_);
    Add(cancelButton_);
}

auto EntryFormComponent::clear() -> void {
    title_.clear();
    error_.clear();
}

auto EntryFormComponent::createElements() -> std::vector<ftxui::Element> {
    using namespace ftxui;
    std::vector<Element> elements;

    elements.push_back(text(formTitle) | bold | center);
    elements.push_back(separator());

    elements.push_back(hbox({
        text(" Title: ") | bold,
        filler(),
        titleInput_->Render() | flex,
        filler(),
    }));

    elements.push_back(separator());

    elements.push_back(hbox({
        filler(),
        submitButton_->Render(),
        text("  "),
        cancelButton_->Render(),
        filler(),
    }));

    if (!error_.empty()) {
        elements.push_back(separator());
        elements.push_back(text(error_) | color(Color::Red) | center);
    }

    elements.push_back(separator());
    elements.push_back(text(formHelpMenu) | dim | center);
    return elements;
}

auto EntryFormComponent::OnRender() -> ftxui::Element {
    using namespace ftxui;

    auto elements{createElements()};
    auto width{Terminal::Size().dimx / 3};

    return vbox(elements) | border | size(WIDTH, EQUAL, width);
}

auto EntryFormComponent::OnEvent(ftxui::Event event) -> bool {
    using namespace ftxui;

    if (event == Event::Escape) {
        onCancel_();
        clear();

        return true;
    }

    return ComponentBase::OnEvent(event);
}
} // namespace hbt::ui::tui
