cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_COLOR_MAKEFILE ON)
option(BUILD_TESTS "Build the tests." OFF)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(cmake/config.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

file(GLOB THIRD_PARTY_INC_DIRS "3rd_party/*/include")

include_directories(${THIRD_PARTY_INC_DIRS} "include")

add_subdirectory(3rd_party)

add_subdirectory(src)
