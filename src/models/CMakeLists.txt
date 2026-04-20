add_library(models_lib STATIC
    src/task.cpp
    src/user.cpp
	src/datetime.cpp
	src/interval.cpp
	src/duration_units.cpp
	src/duration_units_parser.cpp
	src/recurrent_task.cpp
	src/recurrence_patterns.cpp
	src/weekdays.cpp
)

target_include_directories(models_lib PUBLIC include)

target_link_libraries(models_lib PUBLIC nlohmann_json::nlohmann_json)
