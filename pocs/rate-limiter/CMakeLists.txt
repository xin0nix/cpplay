cmake_minimum_required(VERSION 3.20)
project(RateLimiter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RateLimiter
    IpAddress.cpp
    TcpSocket.cpp
    ThreadPool.cpp
    main.cpp
)
