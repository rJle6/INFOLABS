cmake_minimum_required(VERSION 3.10)
project(Laba_blin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(random_vars_lab /home/rJle6/Laba_blin/src/(vstav_chto_nado).cpp)

if(MSVC)
    target_compile_options(random_vars_lab PRIVATE /W4)
else()
    target_compile_options(random_vars_lab PRIVATE -Wall -Wextra -Wpedantic)
endif()
