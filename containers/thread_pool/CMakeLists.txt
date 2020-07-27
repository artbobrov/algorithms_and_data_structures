cmake_minimum_required(VERSION 3.8)

project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wextra-semi \
    -O1 -g -fsanitize=address -fno-omit-frame-pointer")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(thread_pool smoke_test.cpp thread_pool.hpp)
