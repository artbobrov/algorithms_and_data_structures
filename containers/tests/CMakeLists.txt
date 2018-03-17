cmake_minimum_required(VERSION 3.9)
project(tests)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/googletest)

include_directories(lib/googletest/googletest/include ctl/data_structures)
set(SOURCE_FILES vector_tests/contructor_tests.cpp vector_tests/methods_test.cpp vector_tests/extra_functions.hpp ctl/interface/modifiable/bidirectional_element_accessible_modifiable.hpp)


add_executable(tests ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} gtest gtest_main)