cmake_minimum_required(VERSION 3.30)
project(superTicTacToe)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(superTicTacToe WIN32 ./src/main.cpp)

target_include_directories(superTicTacToe PUBLIC "${CMAKE_SOURCE_DIR}/include/")

target_link_libraries(superTicTacToe "${CMAKE_SOURCE_DIR}/libs/x64/SDL2main.lib")
target_link_libraries(superTicTacToe "${CMAKE_SOURCE_DIR}/libs/x64/SDL2.lib")
target_link_libraries(superTicTacToe "${CMAKE_SOURCE_DIR}/libs/x64/SDL2_image.lib")