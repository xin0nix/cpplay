# Find the protoc compiler
find_package(Protobuf REQUIRED)
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

# Add the generated sources to the build
add_library(ExchangeFormatProto STATIC ${PROTO_SRC})
target_include_directories(ExchangeFormatProto
                           PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(ExchangeFormatProto PUBLIC ${Protobuf_LIBRARIES})
