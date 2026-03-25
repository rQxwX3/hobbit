add_subdirectory(implementations/tui)

add_library(ui_lib INTERFACE)

target_include_directories(ui_lib INTERFACE
    interfaces
)
