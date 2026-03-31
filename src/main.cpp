#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>
#include <ranges>
#include <vector>
#include <string>

class UltraGambler : public olc::PixelGameEngine {
public:
    UltraGambler() {
        sAppName = "ULTRA GAMBLER";
    }

private:
    std::map<int, olc::Sprite*> mapSprites;
    std::map<int, olc::Decal*> mapDecals;

    // rando generator seed
    std::mt19937 generateRandomNumber{ std::random_device{}() };
    std::vector<int> randomNumbers;

    void LoadAsset(const int id, const std::string& path) {
        mapSprites[id] = new olc::Sprite(path);
        mapDecals[id] = new olc::Decal(mapSprites[id]);
    }

    bool OnUserCreate() override {
        LoadAsset(0, "./assets/test.png"); // Pinguin als Default
        for (int i = 1; i <= 4; i++) {
            LoadAsset(i, "./assets/" + std::to_string(i) + ".png");
        }
        generateRandomNumbers();
        return true;
    }

    void generateRandomNumbers() {
        std::uniform_int_distribution distribution(1, 4);

        randomNumbers.clear();
        for (int i = 0; i < 15; i++) {
            randomNumbers.push_back(distribution(generateRandomNumber));
        }
    }

    olc::Decal* getDecalFromNumber(const int number) {
        if (mapDecals.contains(number)) return mapDecals[number];
        return mapDecals[0]; // Default Pinguin
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
        for (const auto &val: mapSprites | std::views::values) delete val;
        for (const auto &val: mapDecals | std::views::values)  delete val;
        return true;
    }
};

int main() {
    UltraGambler game;
    if (game.Construct(250, 250, 2, 2))
        game.Start();
    return 0;
}