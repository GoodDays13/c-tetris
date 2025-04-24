cmake_minimum_required(VERSION 3.10)

project(Tetris C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SDL2_STATIC "Static link SDL2" OFF)

add_executable(tetris
	src/main.c
	src/game.c
	src/input.c
	src/renderer.c
	src/audio.c
)

target_include_directories(tetris PUBLIC "${PROJECT_SOURCE_DIR}/include")

find_package(SDL2 REQUIRED)

if(SDL2_STATIC)
	target_link_libraries(tetris ${SDL2_STATIC_LIBRARIES})
else()
	target_link_libraries(tetris ${SDL2_LIBRARIES})
endif()
