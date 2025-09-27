cmake_minimum_required(VERSION 3.10)
project(Blackjack21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Card.cpp
    src/Deck.cpp
    src/Player.cpp
    src/Dealer.cpp
    src/Hand.cpp
)

add_executable(blackjack21 ${SOURCES})
target_include_directories(blackjack21 PRIVATE src)