enable_testing()

cmake_minimum_required(VERSION 3.23...4.1)

project(hobbit)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "
    ========================================================
    This project requires GCC (GNU Compiler Collection)
    Detected compiler: ${CMAKE_CXX_COMPILER_ID}
    
    Please use GCC to compile this project:
      - Install GCC 15 or later
      - Configure CMake with: -DCMAKE_CXX_COMPILER=g++-15
    ========================================================
    ")
endif()

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.0)
    message(FATAL_ERROR "
    ========================================================
    This project requires GCC 14 or later for C++23 support.
    Detected GCC version: ${CMAKE_CXX_COMPILER_VERSION}
    
    Please upgrade your GCC compiler.
    ========================================================
    ")
endif()

add_subdirectory(libs/json)
add_subdirectory(libs/gtest)
add_subdirectory(libs/ftxui)

add_subdirectory(src/models)
add_subdirectory(src/repositories)
add_subdirectory(src/storage)

add_subdirectory(src/core)
add_subdirectory(src/ui)

add_subdirectory(tests)

add_executable(hobbit src/main.cpp)

target_link_libraries(hobbit PRIVATE 
	models_lib 
	repositories_lib 
	storage_lib 
	core_lib
	ui_lib
	nlohmann_json::nlohmann_json
	ftxui::component
    ftxui::dom
    ftxui::screen
)
