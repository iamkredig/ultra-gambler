#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class UltraGambler : public olc::PixelGameEngine {

public:
    bool OnUserCreate() override {
        // Called once at the start, so create things here
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        return true;
    }
};


int main() {
    if (UltraGambler game; game.Construct(128, 128, 4, 4))
        game.Start();

    return 0;
}
