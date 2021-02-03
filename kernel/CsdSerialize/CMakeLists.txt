cmake_minimum_required(VERSION 3.17)
set(ProjectName CsdSerialize)
project(${ProjectName})

set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(tinyxml2 REQUIRED)

include_directories(${ProjectName} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(flatbuffers)
add_subdirectory(reader)

add_library(${ProjectName} FlatBuffersSerialize.cpp ObjectFactory.cpp )
target_link_libraries(${ProjectName} tinyxml2::tinyxml2 reader)


