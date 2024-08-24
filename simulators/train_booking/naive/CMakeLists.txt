cmake_minimum_required(VERSION 3.28.3)
project(CPPlay CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   message("Set up clang includes")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#   set(CMAKE_CXX_STANDARD_LIBRARY "libc++")
# endif()

# TODO: use fetch content instead, to use latest versions
find_package(range-v3 REQUIRED)
find_package(Protobuf REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
