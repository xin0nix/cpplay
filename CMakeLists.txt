cmake_minimum_required(VERSION 3.10)
project(solution C CXX)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


# Fetch GTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

# Fetch GBenchmark
FetchContent_Declare(
  benchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.7.0.zip
)
FetchContent_MakeAvailable(benchmark)

# Add test cpp file
add_executable(solution_test solution_test.cpp solution.cpp)
target_link_libraries(solution_test gtest_main Threads::Threads)

# Add benchmark cpp file
add_executable(solution_benchmark solution_benchmark.cpp solution.cpp)
target_link_libraries(solution_benchmark benchmark::benchmark Threads::Threads)

# Add main cpp file
add_executable(solution main.cpp solution.cpp)
target_link_libraries(solution Threads::Threads)
