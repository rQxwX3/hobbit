add_library(core_lib STATIC
    src/task_manager.cpp
	src/app.cpp
)

target_include_directories(core_lib PUBLIC include)

target_link_libraries(core_lib PUBLIC 
	stduuid
	ui_lib
	models_lib
	repositories_lib
)
