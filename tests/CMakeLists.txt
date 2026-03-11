include(GoogleTest)
message(STATUS "Loading tests from ${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(first_test unit/test_first.cpp)

target_link_libraries(first_test PRIVATE gtest gtest_main)

gtest_discover_tests(first_test)
