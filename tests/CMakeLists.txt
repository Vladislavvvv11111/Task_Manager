add_executable(run_tests test_task_manager.cpp)
target_link_libraries(run_tests PRIVATE task_manager_lib)
target_include_directories(run_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME run_tests COMMAND run_tests)
