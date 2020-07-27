cmake_minimum_required(VERSION 3.8)

project(variant)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wextra-semi -g -fsanitize=address -fno-omit-frame-pointer")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(variant main.cpp variant.hpp)
#add_executable(variant variant_smoke_test.cpp variant.hpp)
#add_executable(variant visitor_smoke_test.cpp variant.hpp)
