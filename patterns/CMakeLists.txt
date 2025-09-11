cmake_minimum_required(VERSION 3.7.2)

project (Patterns CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_LIBRARY "libc++")

include(FetchContent)

file(GLOB_RECURSE PATTERN_SOURCES "*.cpp")

foreach(PATTERN_SRC ${PATTERN_SOURCES})
  get_filename_component(PATTERN_DIR ${PATTERN_SRC} DIRECTORY)
  string(REGEX REPLACE ".*/([^/]+)$" "\\1" PATTERN_NAME ${PATTERN_DIR})
  add_executable(${PATTERN_NAME} ${PATTERN_SRC})
  set_target_properties(${PATTERN_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endforeach()

