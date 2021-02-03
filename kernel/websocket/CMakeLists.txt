cmake_minimum_required(VERSION 3.17)
project(websocket)

set(CMAKE_CXX_STANDARD 20)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
find_package(Boost REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(websocket
        empty.cpp WebSocketServer.cpp Handlers.cpp)
target_link_libraries(websocket  Boost::boost )