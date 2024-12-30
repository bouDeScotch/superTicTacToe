cmake_minimum_required(VERSION 3.30)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled WIN32 ./src/main.cpp)

target_include_directories(untitled PUBLIC "${CMAKE_SOURCE_DIR}/include/")

target_link_libraries(untitled "${CMAKE_SOURCE_DIR}/libs/x64/SDL2main.lib")
target_link_libraries(untitled "${CMAKE_SOURCE_DIR}/libs/x64/SDL2.lib")
