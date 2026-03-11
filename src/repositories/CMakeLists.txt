add_library(repositories_lib STATIC implementations/json_repository.cpp)

target_include_directories(repositories_lib PRIVATE
    interfaces
    implementations
)

target_link_libraries(repositories_lib PRIVATE
	nlohmann_json::nlohmann_json
	storage_lib
)
