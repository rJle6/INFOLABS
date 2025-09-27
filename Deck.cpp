#include "Deck.h"

Deck::Deck() : currentIndex(0) {
    for (int s = 0; s < 4; s++) {
        for (int r = 2; r <= 14; r++) {
            cards.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
        }
    }
    shuffle();
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
    currentIndex = 0;
}

Card Deck::dealCard() {
    if (currentIndex >= cards.size()) {
        reset();
    }
    return cards[currentIndex++];
}

void Deck::reset() {
    shuffle();
}

size_t Deck::cardsLeft() const {
    return cards.size() - currentIndex;
}