cmake_minimum_required(VERSION 3.7)
project(SDL2Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(SDL2_INCLUDE_DIRS $ENV{SDL2_INCLUDE_DIRS})
set(SDL2_LIBRARIES $ENV{SDL2_LIBRARIES})

include_directories(${SDL2_INCLUDE_DIRS} src)

add_executable(SnakeGame
    src/main.cpp
    src/game.cpp
    src/controller.cpp
    src/renderer.cpp
    src/snake.cpp
)

target_compile_definitions(SnakeGame PRIVATE SDL_MAIN_HANDLED)

target_link_libraries(SnakeGame ${SDL2_LIBRARIES})
