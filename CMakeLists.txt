cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(BuildAll LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/tools.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/budoux ${CMAKE_BINARY_DIR}/budoux)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests ${CMAKE_BINARY_DIR}/tests)