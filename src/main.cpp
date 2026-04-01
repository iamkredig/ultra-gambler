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
        float offset = 0.0f;
        bool isSpinning = false;
        int spinCount = 3;
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
        CreateLayer();
        EnableLayer(1, true);
        for (int i = 1; i <= 4; i++) {
            LoadAsset(i, "./assets/" + std::to_string(i) + ".png");
        }
        generateRandomNumbers();
        return true;
    }

    void generateRandomNumbers() {
        std::uniform_int_distribution distribution(1, 4);

        for (auto &[symbols, offset, isSpinning, spinCount] : columns) { // for every column, clear old symbols and generate new ones
            symbols.clear();
            for (int j = 0; j < 4; j++) { // 4 because need new element to roll
                symbols.push_back(distribution(generateRandomNumber));
            }
        }
    }

    olc::Decal* getDecalFromNumber(const int number) {
        if (mapDecals.contains(number)) return mapDecals[number];
        return nullptr;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        SetDrawTarget(1);
        Clear(olc::WHITE);

        if (GetKey(olc::Key::SPACE).bPressed) {
            for (auto &[symbols, offset, isSpinning, spinCount] : columns) {
                isSpinning = true;
                spinCount = 3;
            }
        }
        for (auto &[symbols, offset, isSpinning, spinCount] : columns) { //calculate offset for column and create new symbols
            if (isSpinning) {
                constexpr float symbolHeight = 35.0f;
                constexpr float speed = 0.08f;

                offset += speed;

                if (offset >= symbolHeight) {
                    offset -= symbolHeight;

                    if (!symbols.empty()) symbols.pop_back(); // when traveled far away delete symbol that out of bounds
                    std::uniform_int_distribution dist(1, 4);
                    symbols.insert(symbols.begin(), dist(generateRandomNumber));

                    if (spinCount > 0) {
                        spinCount--;
                    } else {
                        isSpinning = false;
                        offset = 0.0f; // reset the column position for next spin
                    }
                }
            }
        }

        DrawString(10, 10, "press 'space' to gamble!", olc::BLACK);
        for (int i = 0; i < 5; i++) { // 5 columns
            for (int j = 0; j < 4; j++) { // 3 symbols per column + 1 the newly generated one
                constexpr int x_start = 10;
                constexpr int y_start = 40;

                const int symbolID = columns[i].symbols[j];
                olc::Decal* decalPointer = getDecalFromNumber(symbolID);

                float positionX = static_cast<float>(x_start) + (static_cast<float>(i) * 40.0f);
                float positionY = static_cast<float>(y_start) + (static_cast<float>(j) * 35.0f) + columns[i].offset;

                if (decalPointer != nullptr) {
                    DrawDecal({ positionX, positionY }, decalPointer, { 0.3f, 0.3f });
                }
            }
        }

        SetDrawTarget(nullptr);
        Clear(olc::BLANK);
        FillRect({0, 0}, {ScreenWidth(), 70}, olc::WHITE);
        FillRect({0, 175}, {ScreenWidth(), ScreenHeight() - 175}, olc::WHITE);
        SetPixelMode(olc::Pixel::NORMAL);


        DrawString(10, 10, "press 'space' to gamble!", olc::BLACK);

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