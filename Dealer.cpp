#include "Dealer.h"

Dealer::Dealer() : Player("Dealer") {}

bool Dealer::wantsHit() const {
    return getHandValue() < 17;
}

void Dealer::showFirstCard() {
}