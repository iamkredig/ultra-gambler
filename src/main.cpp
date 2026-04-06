#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>

#include "GameState.h"
#include "assets/Assets.h"
#include "logic/Renderer.h"

class UltraGambler : public olc::PixelGameEngine {
public:
    UltraGambler() : state() {
        sAppName = "ULTRA GAMBLER";
    }

    GameState state;
    Assets assets;
    Renderer renderer;

private:
    void startSpinningOnPress() {
        if (state.currentBet <= state.coins && state.currentBet >= 10) {
            if (GetKey(olc::Key::SPACE).bPressed && !state.IsAnyColumnSpinning()) {
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

    void startCountingAfterSpin() {
        if (!state.IsAnyColumnSpinning() && !state.resultCalculated) {
            state.win.CalculateBiggestChain(state.columnPointers);
            state.resultCalculated = true;
            state.coins += state.win.CalculateCoins(state.betAtStart);
            if (state.bestCoins < state.coins) {
                state.bestCoins = state.coins;
            }
        }
    }

    void placeBetOnPress() {
        if (GetKey(olc::Key::LEFT).bPressed) {
            state.currentBetType = 10;
        }
        if (GetKey(olc::Key::RIGHT).bPressed) {
            state.currentBetType = 0;
            state.currentBet = state.coins;
        }

        if (state.currentBetType == 10) {
            if (GetKey(olc::Key::UP).bPressed) {
                state.currentBet += 10;
            }
            if (GetKey(olc::Key::DOWN).bPressed) {
                state.currentBet -= 10;
            }
            if (state.currentBet > state.coins) state.currentBet = state.coins;
            if (state.currentBet < 10) state.currentBet = 10;
        }
        else {
            state.currentBet = state.coins;
        }
    }

    bool OnUserCreate() override {
        CreateLayer();
        EnableLayer(1, true);

        assets.Load();

        for (auto& column : state.columns) {
            state.columnPointers.push_back(&column);
            column.FillRandomNumbers(state.rng);
        }

        return true;
    }

    bool OnUserUpdate(float elapsedTime) override {

        placeBetOnPress();
        startSpinningOnPress();
        state.lever.Animate(elapsedTime);

        for (auto& column : state.columns) {
            column.Update(state.rng, elapsedTime);
        }
        startCountingAfterSpin();

        renderer.Draw(*this, state, assets);

        return true;
    }
};

int main() {
    UltraGambler game;
    if (game.Construct(250, 250, 2, 2))
        game.Start();
    return 0;
}