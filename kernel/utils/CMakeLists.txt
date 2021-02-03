cmake_minimum_required(VERSION 3.17)
project(utils)

set(CMAKE_CXX_STANDARD 20)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(utils
        bundle.cpp
        RequestHelper.cpp
        ResConfigParser.cpp
        Tools.cpp
        FlaExtension.cpp)
target_link_libraries(utils  Boost::boost tinyxml2::tinyxml2)
