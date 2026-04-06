#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>

#include "GameState.h"
#include "assets/Assets.h"
#include "logic/InputHandler.h"
#include "logic/Renderer.h"

class UltraGambler : public olc::PixelGameEngine {
public:
    UltraGambler() : state() {
        sAppName = "ULTRA GAMBLER";
    }

    GameState state;
    Assets assets;
    Renderer renderer;
    InputHandler input;

private:

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

        input.Handle(*this, state);

        for (auto& column : state.columns) {
            column.Update(state.rng, elapsedTime);
        }
        startCountingAfterSpin();

        renderer.Draw(*this, state, assets, elapsedTime);

        return true;
    }
};

int main() {
    UltraGambler game;
    if (game.Construct(250, 250, 2, 2))
        game.Start();
    return 0;
}