add_library(Train ${CMAKE_CURRENT_SOURCE_DIR}/Train.cpp)

target_include_directories(Train PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Train PUBLIC range-v3::range-v3)
