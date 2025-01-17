add_library(ThreadPoolServer OBJECT ThreadPoolServer.cpp)
target_link_libraries(ThreadPoolServer PUBLIC 
    Base
    TcpSocket
    Exchange
    Storage
    Boost::program_options
    Boost::log
    Boost::asio
)
target_include_directories(ThreadPoolServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})