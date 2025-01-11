cmake_minimum_required(VERSION 3.28.3)
project(CPPlay CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)

set(BOOST_VERSION "1.85.0")
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-${BOOST_VERSION}
)
FetchContent_MakeAvailable(Boost)

add_subdirectory(app)
add_subdirectory(lib)
add_subdirectory(test)

enable_testing()
