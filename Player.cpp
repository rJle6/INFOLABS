#include "Player.h"
#include "Card.h"
#include <iostream>

Player::Player(const std::string& playerName) : name(playerName), isActive(true) {}

void Player::addCard(const Card& card) {
    hand.addCard(card);
}

void Player::clearHand() {
    hand.clear();
    isActive = true;
}

int Player::getHandValue() const {
    return hand.getValue();
}

const Hand& Player::getHand() const {
    return hand;
}

std::string Player::getName() const {
    return name;
}

bool Player::getIsActive() const {
    return isActive;
}

void Player::setIsActive(bool active) {
    isActive = active;
}

double Player::calculateWinProbability(const Card& dealerVisibleCard) const {
    int playerValue = getHandValue();
    if (playerValue > 21) return 0.0;
    if (hand.hasBlackjack()) return 0.95;
    
    int dealerVisibleValue = dealerVisibleCard.getValue();
    int difference = playerValue - dealerVisibleValue;
    
    if (difference > 5) return 0.85;
    if (difference > 3) return 0.75;
    if (difference > 0) return 0.65;
    if (difference > -3) return 0.45;
    return 0.25;
}