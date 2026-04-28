include(GoogleTest)

# test models
add_executable(models_test 
	unit/models/test_user.cpp
	unit/models/test_duration.cpp
	unit/models/test_duration_parser.cpp
	unit/models/test_interval.cpp
	unit/models/test_datetime.cpp
	unit/models/test_task_data.cpp
	unit/models/test_singular_task.cpp
	unit/models/test_time.cpp
	unit/models/test_date.cpp
)

target_link_libraries(models_test PRIVATE 
	gtest 
	gmock
	gtest_main 
	nlohmann_json::nlohmann_json
	models_lib
	storage_lib
)

target_link_libraries(models_test PRIVATE gtest 
	gmock
	gtest_main 
	nlohmann_json::nlohmann_json
	models_lib
	storage_lib
)

# test json repos
add_executable(json_repositories_test 
	unit/test_json_repositories.cpp
	unit/util/src/fake_storage_engine.cpp
	unit/util/src/fake_model.cpp
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

target_include_directories(json_repositories_test PRIVATE
	unit/util/include	
)

# test json storage engine
add_executable(json_storage_engine_test unit/test_json_storage_engine.cpp)

target_link_libraries(json_storage_engine_test PRIVATE 
	gtest 
	gmock
	gtest_main 
	nlohmann_json::nlohmann_json
	models_lib
	storage_lib
)

#test task service
add_executable(task_service_test 
	unit/test_task_service.cpp
	unit/util/src/fake_storage_engine.cpp
	unit/util/src/fake_model.cpp
)

target_link_libraries(task_service_test PRIVATE 
	gtest 
	gmock
	gtest_main 
	nlohmann_json::nlohmann_json
	models_lib
	core_lib
	repositories_lib
	storage_lib
)

target_include_directories(task_service_test PRIVATE
	unit/util/include	
)

gtest_discover_tests(models_test)
gtest_discover_tests(json_repositories_test)
gtest_discover_tests(json_storage_engine_test)
gtest_discover_tests(task_service_test)
