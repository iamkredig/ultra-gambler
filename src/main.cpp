#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>
#include <vector>
#include <string>

class UltraGambler : public olc::PixelGameEngine {
public:
    UltraGambler() {
        sAppName = "ULTRA GAMBLER";
    }

private:
    // olc::Sprite* pinguSprite = nullptr;
    // olc::Decal* pinguDecal = nullptr;
    std::vector<int> randomNumbers;

    // rando generator seed
    std::mt19937 generateRandomNumber{ std::random_device{}() };

    bool OnUserCreate() override {
        // pinguSprite = new olc::Sprite("./assets/test.png");
        // pinguDecal = new olc::Decal(pinguSprite);
        return true;
    }

    void generateRandomNumbers() {
        std::uniform_int_distribution distribution(1, 100);

        randomNumbers.clear();
        for (int i = 0; i < 10; i++) {
            randomNumbers.push_back(distribution(generateRandomNumber));
        }
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::VERY_DARK_BLUE);

        if (GetKey(olc::Key::SPACE).bPressed) {
            generateRandomNumbers();
        }

        // if (pinguDecal) DrawDecal({ 0.0f, 0.0f }, pinguDecal);
        DrawString(10, 10, "press 'space' to gamble!", olc::YELLOW);

        for (int i = 0; i < static_cast<int>(randomNumbers.size()); i++) {
            std::string numbersText = "Number " + std::to_string(i + 1) + ": " + std::to_string(randomNumbers[i]);
            DrawString(20, 40 + (i * 20), numbersText, olc::WHITE);
        }
        return true;
    }

    bool OnUserDestroy() override {
        // delete pinguSprite;
        // delete pinguDecal;
        return true;
    }
};

int main() {
    UltraGambler game;
    if (game.Construct(400, 400, 2, 2))
        game.Start();
    return 0;
}