#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"
#include <string>

class Card;

class Player {
protected:
    std::string name;
    Hand hand;
    bool isActive;

public:
    Player(const std::string& playerName);
    virtual ~Player() = default;
    
    virtual bool wantsHit() const = 0;
    void addCard(const Card& card);
    void clearHand();
    int getHandValue() const;
    const Hand& getHand() const;
    std::string getName() const;
    bool getIsActive() const;
    void setIsActive(bool active);
    virtual double calculateWinProbability(const Card& dealerVisibleCard) const;
};

#endif