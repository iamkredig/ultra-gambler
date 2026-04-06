#ifndef ULTRA_GAMBLER_GAMESTATE_H
#define ULTRA_GAMBLER_GAMESTATE_H

#pragma once
#include <vector>
#include <random>

#include "./entities/Column.h"
#include "./entities/Lever.h"
#include "./entities/Win.h"

struct GameState {
    int spins = 0;
    size_t coins = 100;
    size_t bestCoins = coins;

    Column columns[5];
    std::vector<Column*> columnPointers;

    Lever lever;
    Win win;

    bool IsAnyColumnSpinning() {
        return std::ranges::any_of(columns, [](const auto& column) {
            return column.isSpinning;
        });
    }

    bool resultCalculated = true;

    int currentBetType = 10; // 10 is manual, 0 is all in
    size_t currentBet = 10;
    size_t betAtStart = 0; // original bet of the spin

    // rando generator seed
    std::mt19937 rng{ std::random_device{}() };
};

#endif