#include "GameLogic.h"

void StartCountingAfterSpin(GameState &state) {
    if (!state.IsAnyColumnSpinning() && !state.resultCalculated) {
        state.win.CalculateBiggestChain(state.columnPointers);
        state.resultCalculated = true;
        state.coins += state.win.CalculateCoins(state.betAtStart);
        if (state.bestCoins < state.coins) {
            state.bestCoins = state.coins;
        }
    }
}

void GameLogic::Update(GameState &state, float elapsedTime, QuantumRNG &qrng) {
    for (auto &column: state.columns) {
        column.Update(qrng, elapsedTime, state.luckyMode);
    }
    StartCountingAfterSpin(state);
}
