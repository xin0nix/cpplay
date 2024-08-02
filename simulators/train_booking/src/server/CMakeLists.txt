# project(Server)

find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(Server ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp)

target_link_libraries(Server TcpSocket ${Boost_LIBRARIES})
