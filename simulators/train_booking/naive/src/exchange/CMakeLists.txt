# Find the protoc compiler
find_program(PROTOC protoc REQUIRED)

# Generate the C++ sources from the .proto file
set(PROTO_SRC ${CMAKE_CURRENT_BINARY_DIR}/ExchangeFormat.pb.cc)
set(PROTO_HDR ${CMAKE_CURRENT_BINARY_DIR}/ExchangeFormat.pb.h)

add_custom_command(
  OUTPUT ${PROTO_SRC} ${PROTO_HDR}
  COMMAND
    ${PROTOC} ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
    --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ExchangeFormat.proto
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ExchangeFormat.proto)

list(APPEND PROTO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/ExchangeFormat.cpp)

# Add the generated sources to the build
add_library(ExchangeFormat STATIC ${PROTO_SRC})
target_include_directories(ExchangeFormat PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(ExchangeFormat PUBLIC Train)
include_directories(${Train_INCLUDE_DIRS})
