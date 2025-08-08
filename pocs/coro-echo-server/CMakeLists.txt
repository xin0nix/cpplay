cmake_minimum_required(VERSION 3.20)
project(CoroEchoServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CoroEchoServer
    Context.cpp
    Coro.cpp
    IpAddress.cpp
    TcpSocket.cpp
    main.cpp
)
