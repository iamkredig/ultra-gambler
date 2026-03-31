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
    // id (0-4) and sprite/decal
    std::map<int, olc::Sprite*> mapSprites;
    std::map<int, olc::Decal*> mapDecals;

    struct Column {
        std::vector<int> symbols;
    };
    Column columns[5];

    // rando generator seed
    std::mt19937 generateRandomNumber{ std::random_device{}() };
    std::vector<int> randomNumbers;

    void LoadAsset(const int id, const std::string& path) {
        mapSprites[id] = new olc::Sprite(path);
        mapDecals[id] = new olc::Decal(mapSprites[id]);
    }

    bool OnUserCreate() override {
        LoadAsset(0, "./assets/test.png"); // Pinguin als default lol
        for (int i = 1; i <= 4; i++) {
            LoadAsset(i, "./assets/" + std::to_string(i) + ".png");
        }
        generateRandomNumbers();
        return true;
    }

    void generateRandomNumbers() {
        std::uniform_int_distribution distribution(1, 4);

        for (auto &[symbols] : columns) { // for every column, clear old symbols and generate new ones
            symbols.clear();
            for (int j = 0; j < 3; j++) {
                symbols.push_back(distribution(generateRandomNumber));
            }
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

        for (int i = 0; i < 5; i++) { // 5 columns
            for (int j = 0; j < 3; j++) { // 3 symbols per column
                constexpr int x_start = 10;
                constexpr int y_start = 40;

                const int symbolID = columns[i].symbols[j];
                olc::Decal* decalPointer = getDecalFromNumber(symbolID);

                float positionX = static_cast<float>(x_start) + (i * 40.0f);
                float positionY = static_cast<float>(y_start) + (j * 35.0f);

                if (decalPointer != nullptr) {
                    DrawDecal({ positionX, positionY }, decalPointer, { 0.3f, 0.3f });
                }
            }
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