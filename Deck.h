#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <random>
#include <algorithm>

class Deck {
private:
    std::vector<Card> cards;
    size_t currentIndex;

public:
    Deck();
    void shuffle();
    Card dealCard();
    void reset();
    size_t cardsLeft() const;
};

#endif