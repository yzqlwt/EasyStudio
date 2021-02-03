cmake_minimum_required(VERSION 3.17)
set(ProjectName EasyStudio)
project(${ProjectName})

set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ExternalConfig.cmake")

find_package(CURL  REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(minizip REQUIRED)
find_package(tinyxml2 REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/utils)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CsdSerialize)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/websocket)

add_executable(${ProjectName} main.cpp)
target_link_libraries(EasyStudio
        ${OpenCV_LIBS}
        CURL::libcurl
        OpenSSL::SSL
        OpenSSL::Crypto
        Boost::boost
        minizip::minizip
        tinyxml2::tinyxml2
        nlohmann_json::nlohmann_json
        CsdSerialize
        websocket
        utils
    )


message(STATUS "Tips: Copy the third-party library to the output folder")

add_custom_command(TARGET ${ProjectName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/thirdparty/${SYSTEM_STRING}"
        $<TARGET_FILE_DIR:${ProjectName}>)

add_custom_command(TARGET ${ProjectName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/static"
        $<TARGET_FILE_DIR:${ProjectName}>)


