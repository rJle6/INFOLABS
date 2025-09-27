#include "Game.h"
#include "Card.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

Game::Game() : currentPlayerIndex(0) {
    std::srand(std::time(0));
    addPlayer("Alice");
    addPlayer("Bob");
    addPlayer("Charlie");
}

void Game::addPlayer(const std::string& name) {
    class AutoPlayer : public Player {
    public:
        AutoPlayer(const std::string& name) : Player(name) {}
        
        bool wantsHit() const override {
            int value = getHandValue();
            if (value < 12) return true;
            if (value < 15) return (std::rand() % 100) < 70;
            if (value < 17) return (std::rand() % 100) < 40;
            return false;
        }
    };
    
    players.push_back(std::make_unique<AutoPlayer>(name));
}

void Game::startRound() {
    deck.reset();
    dealer.clearHand();
    for (auto& player : players) {
        player->clearHand();
    }
    currentPlayerIndex = 0;
    dealInitialCards();
}

void Game::dealInitialCards() {
    for (int i = 0; i < 2; i++) {
        for (auto& player : players) {
            player->addCard(deck.dealCard());
        }
        dealer.addCard(deck.dealCard());
    }
}

void Game::playRound() {
    displayGameState();
    playerTurns();
    dealerTurn();
    determineWinners();
}

void Game::playerTurns() {
    for (auto& player : players) {
        std::cout << "\n--- Ход " << player->getName() << " ---" << std::endl;
        
        while (player->getIsActive() && !player->getHand().isBusted()) {
            displayGameState();
            
            auto dealerCards = dealer.getHand().getCards();
            if (dealerCards.size() < 2) continue;
            
            Card dealerVisibleCard = dealerCards[1];
            double probability = player->calculateWinProbability(dealerVisibleCard);
            
            std::cout << "Вероятность выигрыша для " << player->getName() 
                      << ": " << (probability * 100) << "%" << std::endl;
            
            if (player->wantsHit()) {
                std::cout << player->getName() << " берет карту..." << std::endl;
                player->addCard(deck.dealCard());
                
                if (player->getHand().isBusted()) {
                    std::cout << player->getName() << " перебрал!" << std::endl;
                    player->setIsActive(false);
                }
            } else {
                std::cout << player->getName() << " пропускает ход." << std::endl;
                player->setIsActive(false);
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void Game::dealerTurn() {
    std::cout << "\n--- Ход дилера ---" << std::endl;
    dealer.showFirstCard();
    displayGameState();
    
    while (dealer.wantsHit() && !dealer.getHand().isBusted()) {
        std::cout << "Дилер берет карту..." << std::endl;
        dealer.addCard(deck.dealCard());
        displayGameState();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void Game::displayGameState() const {
    std::cout << "\n=== ТЕКУЩЕЕ СОСТОЯНИЕ ИГРЫ ===" << std::endl;
    
    std::cout << "Дилер: " << dealer.getHand().toString() 
              << " (очки: " << dealer.getHandValue() << ")" << std::endl;
    
    for (const auto& player : players) {
        std::string status = player->getIsActive() ? "активен" : "пас";
        if (player->getHand().isBusted()) status = "перебор";
        
        std::cout << player->getName() << ": " << player->getHand().toString()
                  << " (очки: " << player->getHandValue() << ") [" << status << "]" << std::endl;
    }
    std::cout << "===============================" << std::endl;
}
void Game::determineWinners() {
    std::cout << "\n=== РЕЗУЛЬТАТЫ РАУНДА ===" << std::endl;
    
    int dealerValue = dealer.getHandValue();
    bool dealerBusted = dealer.getHand().isBusted();
    
    for (const auto& player : players) {
        int playerValue = player->getHandValue();
        bool playerBusted = player->getHand().isBusted();
        
        std::cout << player->getName() << ": ";
        
        if (playerBusted) {
            std::cout << "Проиграл (перебор)";
        } else if (dealerBusted) {
            std::cout << "Выиграл (дилер перебрал)";
        } else if (playerValue > dealerValue) {
            std::cout << "Выиграл";
        } else if (playerValue == dealerValue) {
            std::cout << "Ничья";
        } else {
            std::cout << "Проиграл";
        }
        
        std::cout << " (" << playerValue << " vs " << dealerValue << ")" << std::endl;
    }
}

void Game::playGame() {
    std::cout << "Добро пожаловать в игру 21!" << std::endl;
    
    char continuePlaying = 'y';
    while (continuePlaying == 'y' || continuePlaying == 'Y') {
        startRound();
        playRound();
        
        std::cout << "\nХотите сыграть еще раз? (y/n): ";
        std::cin >> continuePlaying;
    }
    
    std::cout << "Спасибо за игру!" << std::endl;
}