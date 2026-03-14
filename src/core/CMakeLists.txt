add_library(core_lib STATIC
    src/entry_service.cpp
)

target_include_directories(core_lib PUBLIC include)

target_link_libraries(core_lib PRIVATE 
	models_lib
	repositories_lib
)
