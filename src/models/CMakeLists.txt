add_library(models_lib STATIC
    src/user.cpp
	src/datetime.cpp
	src/interval.cpp
	src/duration_units.cpp
	src/duration_units_parser.cpp
	src/task_series.cpp
	src/singular_task.cpp
	src/task_data.cpp
	src/recurrence_patterns.cpp
	src/weekdays.cpp
	# src/task_override.cpp NOT TO BE ADDED IN ALPHA
)

target_include_directories(models_lib PUBLIC include)

target_link_libraries(models_lib PUBLIC 
	nlohmann_json::nlohmann_json 
	core_lib
)
