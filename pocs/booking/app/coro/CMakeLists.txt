add_executable(Coroutines Coroutines.cpp)
target_link_libraries(Coroutines PRIVATE 
    Exchange
    Storage
    Boost::asio
    Boost::log
    Boost::program_options
    Boost::thread
    Boost::system
)
