cmake_minimum_required(VERSION 3.7.2)

project (CPPlay CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_LIBRARY "libc++")

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE solution_test_files "*.cpp")

foreach(solution_test_file ${solution_test_files})
  get_filename_component(solution_dir ${solution_test_file} DIRECTORY)
  string(REGEX REPLACE ".*/([^/]+)$" "\\1" solution_name ${solution_dir})
  add_executable(${solution_name} ${solution_test_file})
  set_target_properties(${solution_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  target_link_libraries(${solution_name} gtest gtest_main gmock)
endforeach()

