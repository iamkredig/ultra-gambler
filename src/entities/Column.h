#ifndef ULTRA_GAMBLER_COLUMN_H
#define ULTRA_GAMBLER_COLUMN_H
#include <random>
#include <vector>

class Column {
public:
    std::vector<int> symbols;
    float offset = 0.0f;
    bool isSpinning = false;
    int spinCount = 0; // 3?

    void Update(std::mt19937& randomNumber, float elapsedTime);

    void FillRandomNumbers(std::mt19937& randomNumber);

    void Spin(int initialSpinCount);
};

#endif //ULTRA_GAMBLER_COLUMN_H