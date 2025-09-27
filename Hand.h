#ifndef HAND_H
#define HAND_H

#include "Card.h"
#include <vector>

class Hand {
private:
    std::vector<Card> cards;

public:
    void addCard(const Card& card);
    int getValue() const;
    void clear();
    size_t getCardCount() const;
    const std::vector<Card>& getCards() const;
    bool isBusted() const;
    bool hasBlackjack() const;
    std::string toString() const;
};

#endif