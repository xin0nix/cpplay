find_package(Boost REQUIRED COMPONENTS program_options)

add_library(BookingService ${CMAKE_CURRENT_SOURCE_DIR}/BookingService.cpp)

target_link_libraries(BookingService ExchangeFormat ${Boost_LIBRARIES})

add_executable(Server ${CMAKE_CURRENT_SOURCE_DIR}/Server.cpp)

target_link_libraries(Server TcpSocket ExchangeFormat BookingService ${Boost_LIBRARIES})
