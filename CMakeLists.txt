cmake_minimum_required(VERSION 3.30)
project(superTicTacToe)

set(CMAKE_CXX_STANDARD 20)

add_executable(superTicTacToe WIN32 ./src/main.cpp)

target_include_directories(superTicTacToe PUBLIC "${CMAKE_SOURCE_DIR}/include/")

find_package(SDL2 REQUIRED)
target_link_libraries(superTicTacToe SDL2::SDL2 SDL2::SDL2main)