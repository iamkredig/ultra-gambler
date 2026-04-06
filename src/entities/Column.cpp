#include "../Column.h"

std::uniform_int_distribution distribution(1, 4);

void Column::Update(std::mt19937& randomNumber, float elapsedTime) { //calculate offset for column and create new symbols
    if (!isSpinning) return;

    constexpr float symbolHeight = 35.0f;
    constexpr float speed = 450.0f;

    offset += speed * elapsedTime;

    if (offset >= symbolHeight) {
        offset -= symbolHeight;

        if (!symbols.empty()) symbols.pop_back(); // when traveled far away delete symbol that out of bounds

        symbols.insert(symbols.begin(), distribution(randomNumber));

        if (spinCount > 0) {
            spinCount--;
        } else {
            isSpinning = false;
            offset = 0.0f; // reset the column position for next spin
        }
    }
}

void Column::FillRandomNumbers(std::mt19937& randomNumber) {
    symbols.clear();
    for (int j = 0; j < 4; j++) { // 4 because need new element to roll 3 + 1
        symbols.push_back(distribution(randomNumber));
    }
}

void Column::Spin(const int initialSpinCount) {
    if (isSpinning) return;
    isSpinning = true;
    spinCount = initialSpinCount;
    offset = 0.0f;
}