// #include <ftxui/component/component.hpp>
// #include <ftxui/component/event.hpp>
//
// #include <task_form_component.hpp>
//
// namespace hbt::ui::tui {
// auto TaskFormComponent::clear() -> void {
//     title_.clear();
//     error_.clear();
// }
//
// auto TaskFormComponent::submit() -> void {
//     if (title_.empty()) {
//         error_ = emptyTitleError;
//     } else if (!intervalInput_->isValid()) {
//         error_ = invalidIntervalError;
//     } else {
//         onSubmit_(title_, intervalInput_->getInterval());
//         clear();
//     }
// }
//
// auto TaskFormComponent::cancel() -> void {
//     clear();
//     onCancel_();
// }
//
// TaskFormComponent::TaskFormComponent(UI::createTaskCallback_t onSubmit,
//                                      onCancelCallback_t onCancel)
//     : onSubmit_{std::move(onSubmit)}, onCancel_{std::move(onCancel)},
//       titleInput_{ftxui::Input(&title_, titleInputPlaceholder)},
//       intervalInput_{ftxui::Make<IntervalInputComponent>()},
//       container_{ftxui::Container::Vertical({titleInput_, intervalInput_})} {
//     Add(container_);
// }
//
// auto TaskFormComponent::OnRender() -> ftxui::Element {
//     using namespace ftxui;
//
//     std::vector<Element> elements{
//         text("Title:") | bold,    titleInput_->Render(),    separator(),
//         text("Interval:") | bold, intervalInput_->Render(),
//     };
//
//     if (!error_.empty()) {
//         elements.push_back(separator());
//         elements.push_back(text(error_) | color(Color::Red) | bold);
//     }
//
//     return vbox(elements) | border;
// }
//
// auto TaskFormComponent::OnEvent(ftxui::Event event) -> bool {
//     using namespace ftxui;
//
//     if (event == Event::Escape) {
//         cancel();
//
//         return true;
//     }
//
//     if (event == Event::Return) {
//         submit();
//
//         return true;
//     }
//
//     if (container_->OnEvent(event)) {
//         return true;
//     }
//
//     return ComponentBase::OnEvent(event);
// }
// } // namespace hbt::ui::tui
