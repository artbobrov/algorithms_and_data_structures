cmake_minimum_required(VERSION 3.9)
project(tests)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/googletest)

include_directories(lib/googletest/googletest/include lib/ctl)
set(SOURCE_FILES vector_tests/contructor_tests.cpp extra extra/extra_std_functions.hpp vector_tests/methods_test.cpp)


add_executable(tests ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} gtest gtest_main)