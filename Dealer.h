#ifndef DEALER_H
#define DEALER_H

#include "Player.h"

class Dealer : public Player {
public:
    Dealer();
    bool wantsHit() const override;
    void showFirstCard();
};

#endif