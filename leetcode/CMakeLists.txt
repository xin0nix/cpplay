cmake_minimum_required(VERSION 3.22.1)

project (CPPlay CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message("Set up clang includes")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  set(CMAKE_CXX_STANDARD_LIBRARY "libc++")
endif()

include(FetchContent)

FetchContent_Declare(
    range-v3
    GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
    GIT_TAG 7e6f34b1e820fb8321346888ef0558a0ec842b8e
)
FetchContent_MakeAvailable(range-v3)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 7d76a231b0e29caf86e68d1df858308cd53b2a66
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE SOLUTION_TEST_FILES "${CMAKE_SOURCE_DIR}/problems/*.cpp")

foreach(SOLUTION_TEST_FILE ${SOLUTION_TEST_FILES})
  get_filename_component(solution_dir ${SOLUTION_TEST_FILE} DIRECTORY)
  string(REGEX REPLACE ".*/([^/]+)$" "\\1" solution_name ${solution_dir})
  add_executable(${solution_name} ${SOLUTION_TEST_FILE})
  set_target_properties(${solution_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  target_link_libraries(${solution_name} gtest gtest_main gmock range-v3::range-v3)
endforeach()

