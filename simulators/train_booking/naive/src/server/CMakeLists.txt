find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(Server ${CMAKE_CURRENT_SOURCE_DIR}/Server.cpp)

target_link_libraries(Server TcpSocket ExchangeFormat ${Boost_LIBRARIES})
