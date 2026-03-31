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
    olc::Sprite* spritePingu = nullptr;
    olc::Decal* decalPingu = nullptr;

    olc::Sprite* sprite1 = nullptr;
    olc::Decal* decal1 = nullptr;

    olc::Sprite* sprite2 = nullptr;
    olc::Decal* decal2 = nullptr;

    olc::Sprite* sprite3 = nullptr;
    olc::Decal* decal3 = nullptr;

    olc::Sprite* sprite4 = nullptr;
    olc::Decal* decal4 = nullptr;

    // rando generator seed
    std::mt19937 generateRandomNumber{ std::random_device{}() };
    std::vector<int> randomNumbers;

    bool OnUserCreate() override {
        spritePingu = new olc::Sprite("./assets/test.png");
        decalPingu = new olc::Decal(spritePingu);

        sprite1 = new olc::Sprite("./assets/1.png");
        decal1 = new olc::Decal(sprite1);

        sprite2 = new olc::Sprite("./assets/2.png");
        decal2 = new olc::Decal(sprite2);

        sprite3 = new olc::Sprite("./assets/3.png");
        decal3 = new olc::Decal(sprite3);

        sprite4 = new olc::Sprite("./assets/4.png");
        decal4 = new olc::Decal(sprite4);
        return true;
    }

    void generateRandomNumbers() {
        std::uniform_int_distribution distribution(1, 4);

        randomNumbers.clear();
        for (int i = 0; i < 15; i++) {
            randomNumbers.push_back(distribution(generateRandomNumber));
        }
    }

    olc::Decal* getDecalFromNumber(const int number) const {
        switch(number) {
            case 1:
                return decal1;
            case 2:
                return decal2;
            case 3:
                return decal3;
            case 4:
                return decal4;
            default:
               return decalPingu;
        }
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::WHITE);

        if (GetKey(olc::Key::SPACE).bPressed) {
            generateRandomNumbers();
        }

        DrawString(10, 10, "press 'space' to gamble!", olc::BLACK);

        int y_offset = 0;
        int randomNumbersIndex = 0;

        for (int i = 0; i < 3; i++) { // 3 rows
            for (int j = 0; j < 5; j++) { // 5 columns

                if (randomNumbersIndex < static_cast<int>(randomNumbers.size())) {
                    const int x_offset = j * 40;
                    olc::Decal* decalPointer = getDecalFromNumber(randomNumbers[randomNumbersIndex]);
                    DrawDecal({ static_cast<float>(20 + x_offset), static_cast<float>(40 + y_offset)}, decalPointer, {0.3f, 0.3f});
                    randomNumbersIndex++;
                }
            }
            y_offset += 30;
        }
        return true;
    }

    bool OnUserDestroy() override {
        delete spritePingu;
        delete sprite1;
        delete sprite2;
        delete sprite3;
        delete sprite4;
        delete decalPingu;
        delete decal1;
        delete decal2;
        delete decal3;
        delete decal4;
        return true;
    }
};

int main() {
    UltraGambler game;
    if (game.Construct(250, 250, 2, 2))
        game.Start();
    return 0;
}