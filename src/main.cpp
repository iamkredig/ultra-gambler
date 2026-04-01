#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>
#include <ranges>
#include <vector>
#include <string>

#include "Column.h"
#include "Lever.h"

class UltraGambler : public olc::PixelGameEngine {
public:
    UltraGambler() {
        sAppName = "ULTRA GAMBLER";
    }

private:
    // id (0-4) and sprite/decal
    std::map<int, olc::Sprite*> mapSprites;
    std::map<int, olc::Decal*> mapDecals;

    int spins = 0;

    Column columns[5];
    Lever lever;

    // rando generator seed
    std::mt19937 generateRandomNumber{ std::random_device{}() };
    std::vector<int> randomNumbers;

    void LoadAsset(const int id, const std::string& path) {
        mapSprites[id] = new olc::Sprite(path);
        mapDecals[id] = new olc::Decal(mapSprites[id]);
    }

    olc::Decal* getDecalFromNumber(const int number) {
        if (mapDecals.contains(number)) return mapDecals[number];
        return nullptr;
    }

    void startSpinningOnPress() {
        if (GetKey(olc::Key::SPACE).bPressed && !IsAnyColumnSpinning()) {
            lever.Pull();
            spins++;

            int extraSpin = 0;
            for (auto& column : columns) {
                column.Spin(10 + (extraSpin * 5));
                extraSpin++;
            }
        }
    }

    bool IsAnyColumnSpinning() {
        for (const auto& column : columns) {
            if (column.isSpinning) return true;
        }
        return false;
    }

    bool OnUserCreate() override {
        CreateLayer();
        EnableLayer(1, true);

        for (int i = 1; i <= 4; i++) { // load sprites and decals
            LoadAsset(10+i, "./assets/lever/lever-" + std::to_string(i) + ".png");
            LoadAsset(i, "./assets/symbols/" + std::to_string(i) + ".png");
        }

        for (auto& column : columns) {
           column.FillRandomNumbers(generateRandomNumber);
        }
        return true;
    }

    void DrawColumns() {
        for (int i = 0; i < 5; i++) { // 5 columns
            for (int j = 0; j < 4; j++) { // 3 symbols per column + 1 the newly generated one
                constexpr int x_start = 12;
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
    }

    void DrawBoundaries() {
        SetDrawTarget(nullptr);
        Clear(olc::BLANK);
        FillRect({0, 0}, {ScreenWidth(), 75}, olc::WHITE);
        FillRect({0, 175}, {ScreenWidth(), ScreenHeight() - 175}, olc::WHITE);
    }

    void DrawLever() {
        olc::Decal* leverDecal = getDecalFromNumber(10 + 1 + lever.frame);
        if (leverDecal != nullptr) {
            DrawDecal({ 213, 90 }, leverDecal, { 0.35f, 0.35f });
        }
    }

    void DrawStringCentered(int y, const std::string& text, olc::Pixel color) {
        int32_t textWidth = text.length() * 8 ;
        int32_t x = (ScreenWidth() - textWidth) / 2;
        DrawString(x, y, text, color);
    }

    bool OnUserUpdate(float elapsedTime) override {
        SetDrawTarget(1);
        Clear(olc::WHITE);

        startSpinningOnPress();
        lever.Animate(elapsedTime);

        for (auto& column : columns) {
            column.Update(generateRandomNumber);
        }

        DrawColumns();
        DrawBoundaries();
        DrawLever();

        DrawStringCentered(15,"press space to gamble", olc::BLACK);
        DrawStringCentered(35,"spins: " + std::to_string(spins), olc::BLACK);
        DrawStringCentered(50,"points: ", olc::BLACK);
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
    if (game.Construct(250, 200, 2, 2))
        game.Start();
    return 0;
}