cmake_minimum_required(VERSION 3.9)
project(containers)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tests/lib/googletest)

include_directories(tests/lib/googletest/googletest/include ctl/data_structures)
set(SOURCE_FILES
        tests/list_tests/constructor_tests.cpp
        tests/vector_tests/contructor_tests.cpp
        tests/vector_tests/methods_test.cpp
        tests/list_tests/methods_test.cpp
        tests
        ctl)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} gtest gtest_main)