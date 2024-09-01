cmake_minimum_required(VERSION 3.28.3)
project(CPPlay CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO: use fetch content instead, to use latest versions
find_package(range-v3 REQUIRED)
find_package(Protobuf REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
