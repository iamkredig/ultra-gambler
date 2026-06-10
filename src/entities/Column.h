#ifndef ULTRA_GAMBLER_COLUMN_H
#define ULTRA_GAMBLER_COLUMN_H

#pragma once
#include <random>
#include <vector>
#include "../logic/QuantumRNG.h"

class Column {
public:
    std::vector<int> symbols;
    float offset = 0.0f;
    bool isSpinning = false;
    int spinCount = 0; // 3?

    void Update(QuantumRNG &qrng, float elapsedTime);

    void FillRandomNumbers(QuantumRNG &qrng);

    void Spin(int initialSpinCount);
};

#endif //ULTRA_GAMBLER_COLUMN_H
