add_library(ui_lib STATIC implementations/src/tui.cpp)

target_include_directories(ui_lib PUBLIC
	implementations/include
	interfaces
)

target_link_libraries(ui_lib PRIVATE
	models_lib
	ftxui::component
    ftxui::dom
    ftxui::screen
)
