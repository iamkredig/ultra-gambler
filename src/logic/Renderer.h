#ifndef ULTRA_GAMBLER_RENDERER_H
#define ULTRA_GAMBLER_RENDERER_H

#pragma once
#include "olcPixelGameEngine.h"
#include "../GameState.h"
#include "../assets/Assets.h"

class Renderer {
public:
    static void Draw(olc::PixelGameEngine& engine, GameState& state, Assets& assets);
};

#endif