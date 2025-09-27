#include "Hand.h"

void Hand::addCard(const Card& card) {
    cards.push_back(card);
}

int Hand::getValue() const {
    int value = 0;
    int aces = 0;
    
    for (const auto& card : cards) {
        if (card.isVisible()) {
            int cardValue = card.getValue();
            value += cardValue;
            if (cardValue == 11) aces++;
        }
    }
    
    while (value > 21 && aces > 0) {
        value -= 10;
        aces--;
    }
    
    return value;
}

void Hand::clear() {
    cards.clear();
}

size_t Hand::getCardCount() const {
    return cards.size();
}

const std::vector<Card>& Hand::getCards() const {
    return cards;
}

bool Hand::isBusted() const {
    return getValue() > 21;
}

bool Hand::hasBlackjack() const {
    return getCardCount() == 2 && getValue() == 21;
}

std::string Hand::toString() const {
    std::string result;
    for (const auto& card : cards) {
        result += card.toString() + " ";
    }
    return result;
}