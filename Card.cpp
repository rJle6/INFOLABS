#include "Card.h"

Card::Card(Suit s, Rank r) : suit(s), rank(r), visible(true) {}

int Card::getValue() const {
    if (rank == Rank::JACK or rank == Rank::QUEEN or rank == Rank::KING) {
        return 10;
    } else if (rank == Rank::ACE) {
        return 11;
    }
    return static_cast<int>(rank);
}

std::string Card::toString() const {
    if (!visible) return "XX";
    
    std::string result;
    
    switch (rank) {
        case Rank::TWO: result = "2"; break;
        case Rank::THREE: result = "3"; break;
        case Rank::FOUR: result = "4"; break;
        case Rank::FIVE: result = "5"; break;
        case Rank::SIX: result = "6"; break;
        case Rank::SEVEN: result = "7"; break;
        case Rank::EIGHT: result = "8"; break;
        case Rank::NINE: result = "9"; break;
        case Rank::TEN: result = "10"; break;
        case Rank::JACK: result = "J"; break;
        case Rank::QUEEN: result = "Q"; break;
        case Rank::KING: result = "K"; break;
        case Rank::ACE: result = "A"; break;
    }
    
    switch (suit) {
        case Suit::HEARTS: result += "♥️"; break;
        case Suit::DIAMONDS: result += "♦️"; break;
        case Suit::CLUBS: result += "♣️"; break;
        case Suit::SPADES: result += "♠️"; break;
    }
    
    return result;
}

void Card::setVisible(bool v) { visible = v; }
bool Card::isVisible() const { return visible; }
Suit Card::getSuit() const { return suit; }
Rank Card::getRank() const { return rank; }