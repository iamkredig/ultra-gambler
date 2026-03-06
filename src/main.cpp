#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct Pipe {
    float x{};
    float gapY{};
    float width = 20.0f;
};

class FlappyBird : public olc::PixelGameEngine {

    static void UpdatePhysics(float dt) {
        /* Schwerkraft & Bewegung hier */
    }
    static void CheckCollisions() {
        /* Prüfe ob Vogel die Röhre berührt */
    }
    static void DrawEverything() {
        // Zeichne Vogel & Röhren
    }

private:
    float fBirdY = 100;
    float fBirdVel = 0;

    std::list<Pipe> listPipes;

public:
    FlappyBird() { sAppName = "FlappyBird"; }

public:
    bool OnUserCreate() override {
        // Called once at the start, so create things here
        fBirdY = ScreenHeight() / 2.0f;
        fBirdVel = 0.0f;
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        UpdatePhysics(fElapsedTime);
        CheckCollisions();
        DrawEverything();
        return true;
    }
};


int main() {
    if (FlappyBird game; game.Construct(128, 128, 4, 4))
        game.Start();

    return 0;
}
