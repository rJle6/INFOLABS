#include "Game.h"
#include <iostream>

int main() {
    try {
        Game game;
        game.playGame();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}