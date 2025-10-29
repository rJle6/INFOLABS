cmake_minimum_required(VERSION 3.20)
project(SFML_Buttons)

set(CMAKE_CXX_STANDARD 17)

# Загрузка SFML 3.0.2
include(FetchContent)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2
)
FetchContent_MakeAvailable(sfml)

# Добавить include директорию
include_directories(include)

# Добавить исполняемый файл
add_executable(main 
    src/main.cpp
    src/Button.cpp  
    src/Layout.cpp
)

# Копировать шрифты
file(COPY fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Связать с SFML
target_link_libraries(main 
    sfml-graphics 
    sfml-window 
    sfml-system
)