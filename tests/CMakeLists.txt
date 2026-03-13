include(GoogleTest)

add_executable(models_test unit/test_models.cpp)

add_executable(json_repositories_test 
	unit/test_json_repositories.cpp
	unit/util/src/fake_storage_engine.cpp
	unit/util/src/fake_model.cpp
)

target_link_libraries(models_test PRIVATE 
	gtest 
	gmock
	gtest_main 
	nlohmann_json::nlohmann_json
	models_lib
	storage_lib
)

target_include_directories(json_repositories_test PRIVATE
	unit/util/include	
)

target_link_libraries(json_repositories_test PRIVATE
	gtest 
	gmock
	gtest_main 
	nlohmann_json::nlohmann_json
	models_lib
	storage_lib
	repositories_lib
) 

gtest_discover_tests(models_test)
gtest_discover_tests(json_repositories_test)
