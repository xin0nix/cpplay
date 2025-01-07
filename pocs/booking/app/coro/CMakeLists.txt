add_executable(Coroutines Coroutines.cpp)
target_link_libraries(Coroutines Exchange Storage Boost::program_options Boost::log Boost::asio)
