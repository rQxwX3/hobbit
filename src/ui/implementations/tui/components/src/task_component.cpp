// #include <task_component.hpp>
//
// namespace hbt::ui::tui {
// TaskComponent::TaskComponent(const hbt::mods::Task &task)
//     : title_{task.getTitle()}, selected_{false} {}
//
// auto TaskComponent::setSelected(bool selected) -> void { selected_ =
// selected; }
//
// auto TaskComponent::OnRender() -> ftxui::Element {
//     using namespace ftxui;
//
//     auto task{hbox({text(title_)})};
//
//     if (selected_) {
//         return task | border | bold;
//     }
//
//     return task | border;
// }
// } // namespace hbt::ui::tui
