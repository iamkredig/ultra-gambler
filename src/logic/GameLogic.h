#ifndef ULTRA_GAMBLER_GAMELOGIC_H
#define ULTRA_GAMBLER_GAMELOGIC_H

#pragma once
#include "../GameState.h"
#include "./QuantumRNG.h"

class GameLogic {
public:
    void Update(GameState &state, float elapsedTime, QuantumRNG &qrng);
};


#endif
