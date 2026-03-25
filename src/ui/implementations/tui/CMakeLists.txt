add_library(tui_lib STATIC
    src/tui.cpp
    components/src/entry_component.cpp
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
