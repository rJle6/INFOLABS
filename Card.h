#ifndef CARD_H
#define CARD_H

#include <string>

enum class Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
enum class Rank { 
    TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING, ACE
};

class Card {
private:
    Suit suit;
    Rank rank;
    bool visible;

public:
    Card(Suit s, Rank r);
    
    int getValue() const;
    std::string toString() const;
    void setVisible(bool v);
    bool isVisible() const;
    Suit getSuit() const;
    Rank getRank() const;
};

#endif
