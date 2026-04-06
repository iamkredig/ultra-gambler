#include "InputHandler.h"

void HandleSpin(olc::PixelGameEngine &engine, GameState &state) {
    if (state.currentBet <= state.coins && state.currentBet >= 10) {
        if (engine.GetKey(olc::Key::SPACE).bPressed && !state.IsAnyColumnSpinning()) {
            state.lever.Pull();
            state.spins++;
            state.resultCalculated = false;

            state.betAtStart = state.currentBet;

            int extraSpin = 0;
            for (auto &column: state.columns) {
                column.Spin(10 + (extraSpin * 5));
                extraSpin++;
            }
            state.coins -= state.currentBet;
        }
    }
}

void HandleReset(olc::PixelGameEngine &engine, GameState &state) {
    if (engine.GetKey(olc::Key::R).bPressed && state.coins < 10 && !state.IsAnyColumnSpinning()) {
        state.coins = 100;
        state.bestCoins = state.coins;
        state.spins = 0;
        state.currentBet = 10;
    }
};

void HandleBet(olc::PixelGameEngine &engine, GameState &state) {
    if (engine.GetKey(olc::Key::LEFT).bPressed) {
        state.currentBetType = 10;
    }
    if (engine.GetKey(olc::Key::RIGHT).bPressed) {
        state.currentBetType = 0;
        state.currentBet = state.coins;
    }

    if (state.currentBetType == 10) {
        if (engine.GetKey(olc::Key::UP).bPressed) {
            state.currentBet += 10;
        }
        if (engine.GetKey(olc::Key::DOWN).bPressed) {
            state.currentBet -= 10;
        }
        if (state.currentBet > state.coins) state.currentBet = state.coins;
        if (state.currentBet < 10) state.currentBet = 10;
    } else {
        state.currentBet = state.coins;
    }
}

void InputHandler::Handle(olc::PixelGameEngine &engine, GameState &state) {
    HandleBet(engine, state);
    HandleSpin(engine, state);
    HandleReset(engine, state);
}
