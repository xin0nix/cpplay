add_library(CoroutineServer OBJECT CoroutineServer.cpp)
target_link_libraries(CoroutineServer PUBLIC
    Base
    Exchange
    Storage
    Boost::asio
    Boost::log
    Boost::program_options
    Boost::thread
    Boost::system
)
target_include_directories(CoroutineServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
