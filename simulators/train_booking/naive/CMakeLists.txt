cmake_minimum_required(VERSION 3.22.1)

project(CPPlay CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message("Set up clang includes")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  set(CMAKE_CXX_STANDARD_LIBRARY "libc++")
endif()

add_subdirectory(src)

enable_testing()
