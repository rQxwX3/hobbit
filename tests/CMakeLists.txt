include(GoogleTest)

add_executable(models_test unit/test_models.cpp)

target_link_libraries(models_test PRIVATE 
	gtest 
	gtest_main 
	nlohmann_json::nlohmann_json
	models_lib
	storage_lib
	repositories_lib
)

gtest_discover_tests(models_test)
