add_library(models_lib STATIC
    src/entry.cpp
    src/user.cpp
	src/occurrence.cpp
	src/date.cpp
)

target_include_directories(models_lib PUBLIC include)

target_link_libraries(models_lib PUBLIC nlohmann_json::nlohmann_json)
