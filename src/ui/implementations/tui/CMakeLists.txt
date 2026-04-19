add_library(tui_lib STATIC
    src/tui.cpp
    components/src/task_component.cpp
	components/src/task_list_component.cpp
	components/src/task_form_component.cpp
	components/src/orchestrator_component.cpp
	components/src/interval_input_component.cpp
)

target_include_directories(tui_lib PUBLIC
    include
    components/include
)

target_link_libraries(tui_lib PUBLIC
	ui_lib
    models_lib
    ftxui::component
    ftxui::dom
    ftxui::screen
)
