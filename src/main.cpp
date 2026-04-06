#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>
#include <ranges>
#include <vector>
#include <string>

#include "entities/Column.h"
#include "entities/Lever.h"
#include "entities/Win.h"

class UltraGambler : public olc::PixelGameEngine {
public:
    UltraGambler() : win() {
        sAppName = "ULTRA GAMBLER";
    }

private:
    // id (0-4) and sprite/decal
    std::map<int, olc::Sprite*> mapSprites;
    std::map<int, olc::Decal*> mapDecals;

    int spins = 0;
    size_t coins = 100;
    size_t bestCoins = 0;

    Column columns[5];
    std::vector<Column*> columnPointers;

    Lever lever;

    Win win;
    bool resultCalculated = true;

    int currentBetType = 10;
    size_t currentBet = 0;
    size_t betAtStart = 0;

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
        if (currentBet <= coins & currentBet >= 10) {
            if (GetKey(olc::Key::SPACE).bPressed && !IsAnyColumnSpinning()) {
                lever.Pull();
                spins++;
                resultCalculated = false;

                betAtStart = currentBet;

                int extraSpin = 0;
                for (auto &column: columns) {
                    column.Spin(10 + (extraSpin * 5));
                    extraSpin++;
                }
                coins -= currentBet;
            }
        }
    }

    void startCountingAfterSpin() {
        if (!IsAnyColumnSpinning() && !resultCalculated) {
            win.CalculateBiggestChain(columnPointers);
            resultCalculated = true;
            coins += win.CalculateCoins(betAtStart);
        }
    }

    bool IsAnyColumnSpinning() {
        return std::ranges::any_of(columns, [](const auto& column) {
            return column.isSpinning;
        });
    }

    void placeBetOnPress() {
        if (GetKey(olc::Key::LEFT).bPressed) {
            currentBetType = 10;
        }
        if (GetKey(olc::Key::RIGHT).bPressed) {
            currentBetType = 0;
            currentBet = coins;
        }

        if (currentBetType == 10) {
            if (GetKey(olc::Key::UP).bPressed) {
                currentBet += 10;
            }
            if (GetKey(olc::Key::DOWN).bPressed) {
                currentBet -= 10;
            }
            if (currentBet > coins) currentBet = coins;
            if (currentBet < 10) currentBet = 10;
        }
        else {
            currentBet = coins;
        }
    }

    bool OnUserCreate() override {
        CreateLayer();
        EnableLayer(1, true);

        for (int i = 1; i <= 4; i++) { // load sprites and decals
            LoadAsset(10+i, "./assets/lever/lever-" + std::to_string(i) + ".png");
            LoadAsset(i, "./assets/symbols/" + std::to_string(i) + ".png");
        }
        LoadAsset(21, "./assets/21.png");
        LoadAsset(22, "./assets/22.png");

        for (auto& column : columns) {
            columnPointers.push_back(&column);
            column.FillRandomNumbers(generateRandomNumber);
        }

        return true;
    }

    void DrawColumns() {
        for (int i = 0; i < 5; i++) { // 5 columns
            for (int j = 0; j < 4; j++) { // 3 symbols per column + 1 the newly generated one
                constexpr int x_start = 14;
                constexpr int y_start = 102;

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
        FillRect({0, 0}, {ScreenWidth(), 137}, olc::WHITE);
        FillRect({0, 240}, {ScreenWidth(), ScreenHeight() - 240}, olc::WHITE);
    }

    void DrawLever() {
        olc::Decal* leverDecal = getDecalFromNumber(10 + 1 + lever.frame);
        if (leverDecal != nullptr) {
            DrawDecal({ 215, 122 }, leverDecal, { 0.3f, 0.3f });
        }
    }

    void DrawStringCentered(int y, const std::string& text, olc::Pixel color) {
        int textWidth = static_cast<int>(text.length() * 8) ;
        int x = (ScreenWidth() - textWidth) / 2;
        DrawString(x, y, text, color);
    }

    void DrawValueWithIcon(int y, int iconID, const std::string& value, olc::Pixel color) {
        olc::Decal* icon = getDecalFromNumber(iconID);
        if (!icon) return;

        float iconWidth = 16.0f;
        float spacing = 20.0f;

        DrawDecal({ 14, static_cast<float>(y - 12) }, icon, { 0.3f, 0.3f });
        DrawString(static_cast<int>(14.0f + iconWidth + spacing), y, value, color);
    }

    void DrawWinLines() {
        if (IsAnyColumnSpinning()) return;
        for (auto& coord : win.chainCoordinates) {

            int column = coord.first;
            int row = coord.second + 1; // cuz 4 images loaded, only 3 seen

            // same calculations as in column
            float positionX = 14.0f + (static_cast<float>(column) * 40.0f);
            float positionY = 102.0f + (static_cast<float>(row) * 35.0f) + columns[column].offset;

            FillRectDecal({ positionX, positionY }, { 31, 31 }, olc::Pixel(10, 20, 200, 50));
        }
    }

    bool OnUserUpdate(float elapsedTime) override {

        if (coins < 10 && !IsAnyColumnSpinning()) {
            SetDrawTarget(1);
            Clear(olc::BLANK);

            SetDrawTarget(nullptr);
            Clear(olc::WHITE);

            DrawString(53,85, "GAME OVER", olc::RED, 2);
            DrawStringCentered(145, "PRESS R TO RESTART", olc::BLACK);

            if (GetKey(olc::Key::R).bPressed) {
                coins = 100;
                spins = 0;
                currentBet = 10;
                currentBetType = 10;
            }
            return true;
        } else {


        SetDrawTarget(1);
        Clear(olc::WHITE);

        placeBetOnPress();
        startSpinningOnPress();
        lever.Animate(elapsedTime);

        for (auto& column : columns) {
            column.Update(generateRandomNumber, elapsedTime);
        }
        startCountingAfterSpin();

        DrawColumns();
        DrawBoundaries();
        DrawLever();
        DrawWinLines();

        DrawStringCentered(10, "PLACE A BET - ARROW KEYS &", olc::BLACK);
        DrawStringCentered(20, "PRESS SPACE TO GAMBLE", olc::BLACK);
        DrawValueWithIcon(60, 21, std::to_string(coins), olc::BLACK);
        DrawValueWithIcon(96, 22, std::to_string(spins), olc::BLACK);

        DrawString(105, 60, "PLACE YOUR BET", olc::BLACK);

        DrawString(110, 93, std::to_string(currentBet), olc::BLACK);
        FillTriangle(117, 80, 117 - 4, 80 + 6, 117 + 4, 80 + 6, olc::BLACK);
        FillTriangle(117, 106 + 6, 117 - 4, 106, 117 + 4, 106, olc::BLACK);

        DrawString(160, 93, "ALL IN", olc::BLACK);

        if (currentBetType == 10) {
            FillRectDecal({ 105, 90 }, { 34, 15 }, olc::Pixel(10, 20, 200, 50));
        } else {
            FillRectDecal({ 155, 90 }, { 58, 15 }, olc::Pixel(10, 20, 200, 50));
        }

        return true;
        }
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