#ifndef ULTRA_GAMBLER_INPUTHANDLER_H
#define ULTRA_GAMBLER_INPUTHANDLER_H


#pragma once
#include "olcPixelGameEngine.h"
#include "../GameState.h"

class InputHandler {
public:
    void Handle(olc::PixelGameEngine& pge, GameState& state);
};


#endif