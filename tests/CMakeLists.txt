include(GoogleTest)

add_executable(first_test unit/test_first.cpp)

target_link_libraries(first_test PRIVATE gtest gtest_main)

target_include_directories(first_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/libs/gtest/googletest/include
)

gtest_discover_tests(first_test)
