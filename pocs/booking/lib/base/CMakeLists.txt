add_library(Base OBJECT Base.cpp)
target_link_libraries(Base PUBLIC
    Storage
    Boost::asio
    Boost::thread
    Boost::system
)
target_include_directories(Base 
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)