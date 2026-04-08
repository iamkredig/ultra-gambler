#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
#include "GameState.h"
#include "assets/Assets.h"
#include "logic/GameLogic.h"
#include "logic/InputHandler.h"
#include "logic/Renderer.h"

#include <unistd.h>  // chdir
#include <libgen.h>  // dirname

class UltraGambler : public olc::PixelGameEngine {
public:
    UltraGambler() : state() {
        sAppName = "ULTRA GAMBLER";
    }

    GameState state;
    Assets assets;
    Renderer renderer;
    InputHandler input;
    GameLogic logic;

    private:
    bool OnUserCreate() override {
        CreateLayer(); // for game over overlay
        EnableLayer(1, true);

        assets.Load();

        for (auto &column: state.columns) {
            state.columnPointers.push_back(&column);
            column.FillRandomNumbers(state.rng);
        }
        return true;
    }

    bool OnUserUpdate(float elapsedTime) override {
        input.Handle(*this, state);
        logic.Update(state, elapsedTime);
        renderer.Draw(*this, state, assets, elapsedTime);
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc > 0) { //fixes assets path problems on MAC
        // argv[0] is the executable path
        // we need only the directory name
        char* dir = dirname(argv[0]);
        // change workdir to that directory
        chdir(dir);
    }

    UltraGambler game;
    if (game.Construct(250, 250, 2, 2))
        game.Start();
    return 0;
}