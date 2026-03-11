add_library(repositories_lib STATIC
    implementations/src/json_repository.cpp
)

target_include_directories(repositories_lib PUBLIC
    interfaces
    implementations/include
)
