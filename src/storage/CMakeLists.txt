add_library(storage_lib STATIC implementations/src/json_storage_engine.cpp)

target_include_directories(storage_lib PUBLIC
	implementations/include
	interfaces
)

target_link_libraries(storage_lib PRIVATE nlohmann_json::nlohmann_json)
