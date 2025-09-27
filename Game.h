#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Dealer.h"
#include "Deck.h"
#include <vector>
#include <memory>

class Game {
private:
    std::vector<std::unique_ptr<Player>> players;
    Dealer dealer;
    Deck deck;
    int currentPlayerIndex;

public:
    Game();
    void addPlayer(const std::string& name);
    void startRound();
    void playRound();
    void displayGameState() const;
    bool isRoundOver() const;
    void determineWinners();
    void playGame();

private:
    void dealInitialCards();
    void playerTurns();
    void dealerTurn();
};

#endif