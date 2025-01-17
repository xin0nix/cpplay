add_subdirectory(coro)
add_subdirectory(thread_pool)
add_executable(App main.cpp)
target_link_libraries(App 
    Base
    ThreadPoolServer
    CoroutineServer
    Boost::log
    Boost::program_options
)