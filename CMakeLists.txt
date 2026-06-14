cmake_minimum_required(VERSION 3.10)
project(TaskManager CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная библиотека с логикой программы (используется и в приложении, и в тестах)
add_library(task_manager_lib src/task_manager.cpp)
target_include_directories(task_manager_lib PUBLIC include)

# Основное консольное приложение
add_executable(task_manager src/main.cpp)
target_link_libraries(task_manager PRIVATE task_manager_lib)

# Тесты
enable_testing()
add_subdirectory(tests)
