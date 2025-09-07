cmake_minimum_required(VERSION 3.20)

project(ASCII-Tetris LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(tetris ${CMAKE_CURRENT_SOURCE_DIR}/tetris.c ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

target_include_directories(tetris PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Curses REQUIRED)
if(CURSES_FOUND)
    target_link_libraries(tetris PRIVATE ${CURSES_LIBRARIES})
    target_include_directories(tetris PRIVATE ${CURSES_INCLUDE_DIR})
else()
    target_link_libraries(tetris PRIVATE curses)
endif()