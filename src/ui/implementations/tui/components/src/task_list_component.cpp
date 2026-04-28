// #include <task_component.hpp>
// #include <task_list_component.hpp>
//
// #include <ftxui/component/event.hpp>
//
// namespace hbt::ui::tui {
// [[nodiscard]] auto TaskListComponent::getMaxIndex() const -> index_t {
//     auto childrenSize{static_cast<index_t>(children_.size())};
//
//     return (childrenSize == 0) ? noSelectionIndex : childrenSize;
// }
//
// [[nodiscard]] auto TaskListComponent::isSafeIndex(index_t index) const ->
// bool {
//     if (index < 0) {
//         return false;
//     }
//
//     if (auto maxIndex{getMaxIndex()}; maxIndex != noSelectionIndex) {
//         return index < maxIndex;
//     }
//
//     return false;
// }
//
// auto TaskListComponent::updateSelection(index_t newSelectedIndex) -> void {
//     if (!isSafeIndex(newSelectedIndex)) {
//         return;
//     }
//
//     if (isSafeIndex(selectedIndex_)) {
//         children_[selectedIndex_]->setSelected(false);
//     }
//
//     children_[newSelectedIndex]->setSelected(true);
//     selectedIndex_ = newSelectedIndex;
// }
//
// auto TaskListComponent::setTasks(const std::vector<hbt::mods::Task> &tasks)
//     -> void {
//     children_.clear();
//
//     for (const auto &task : tasks) {
//         auto child{std::make_shared<TaskComponent>(task)};
//
//         children_.push_back(child);
//         TaskListComponent::Add(child);
//     }
//
//     updateSelection(tasks.empty() ? noSelectionIndex : 0);
// }
//
// auto TaskListComponent::OnRender() -> ftxui::Element {
//     using namespace ftxui;
//
//     std::vector<Element> elements;
//     elements.reserve(children_.size());
//
//     for (auto &child : children_) {
//         elements.push_back(child->Render());
//     }
//
//     auto width{Terminal::Size().dimx / 3};
//     auto list{vbox(elements) | size(WIDTH, EQUAL, width)};
//
//     return center(list) | flex;
// }
//
// auto TaskListComponent::OnEvent(ftxui::Event event) -> bool {
//     using namespace ftxui;
//
//     if (event == Event::Character('k') || event == Event::ArrowUp) {
//         updateSelection(selectedIndex_ - 1);
//
//         return true;
//     }
//
//     if (event == Event::Character('j') || event == Event::ArrowDown) {
//         updateSelection(selectedIndex_ + 1);
//
//         return true;
//     }
//
//     return ComponentBase::OnEvent(event);
// }
// } // namespace hbt::ui::tui
