include(FetchContent)

FetchContent_Declare(
  gtest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip)
FetchContent_MakeAvailable(gtest)

FetchContent_Declare(
  gmock
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
  SOURCE_SUBDIR googletest-1.15.2/googlemock)
FetchContent_MakeAvailable(gmock)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*Test.cpp")

add_executable(UnitTests main.cpp ${TEST_SOURCES})

target_link_libraries(UnitTests PRIVATE
  Exchange
  gtest_main
  gmock_main
)

# Add test
add_test(NAME UnitTests COMMAND UnitTests)

# Optionally, add a custom target to run all tests
add_custom_target(all_tests ALL DEPENDS UnitTests)
add_custom_command(
  TARGET all_tests
  POST_BUILD
  COMMAND ctest ARGS --output-on-failure
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})