cmake_minimum_required(VERSION 3.17)
project(reader)

set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} reader_src)
add_library(reader ${reader_src})
target_link_libraries(reader tinyxml2::tinyxml2)