cmake_minimum_required(VERSION 3.23...4.1)

project(hobbit)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/models)
add_subdirectory(src/repositories)
add_subdirectory(src/storage)

add_executable(hobbit src/main.cpp)

target_link_libraries(hobbit PRIVATE models_lib repositories_lib storage_lib)
