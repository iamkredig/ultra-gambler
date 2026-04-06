#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <ranges>
#include <string>

#include "GameState.h"
#include "assets/Assets.h"

class UltraGambler : public olc::PixelGameEngine {
public:
    UltraGambler() : state() {
        sAppName = "ULTRA GAMBLER";
    }

    GameState state;
    Assets assets;

private:

    void startSpinningOnPress() {
        if (state.currentBet <= state.coins & state.currentBet >= 10) {
            if (GetKey(olc::Key::SPACE).bPressed && !IsAnyColumnSpinning()) {
                state.lever.Pull();
                state.spins++;
                state.resultCalculated = false;

                state.betAtStart = state.currentBet;

                int extraSpin = 0;
                for (auto &column: state.columns) {
                    column.Spin(10 + (extraSpin * 5));
                    extraSpin++;
                }
                state.coins -= state.currentBet;
            }
        }
    }

    void startCountingAfterSpin() {
        if (!IsAnyColumnSpinning() && !state.resultCalculated) {
            state.win.CalculateBiggestChain(state.columnPointers);
            state.resultCalculated = true;
            state.coins += state.win.CalculateCoins(state.betAtStart);
        }
    }

    bool IsAnyColumnSpinning() {
        return std::ranges::any_of(state.columns, [](const auto& column) {
            return column.isSpinning;
        });
    }

    void placeBetOnPress() {
        if (GetKey(olc::Key::LEFT).bPressed) {
            state.currentBetType = 10;
        }
        if (GetKey(olc::Key::RIGHT).bPressed) {
            state.currentBetType = 0;
            state.currentBet = state.coins;
        }

        if (state.currentBetType == 10) {
            if (GetKey(olc::Key::UP).bPressed) {
                state.currentBet += 10;
            }
            if (GetKey(olc::Key::DOWN).bPressed) {
                state.currentBet -= 10;
            }
            if (state.currentBet > state.coins) state.currentBet = state.coins;
            if (state.currentBet < 10) state.currentBet = 10;
        }
        else {
            state.currentBet = state.coins;
        }
    }

    bool OnUserCreate() override {
        CreateLayer();
        EnableLayer(1, true);

        assets.Load();

        for (auto& column : state.columns) {
            state.columnPointers.push_back(&column);
            column.FillRandomNumbers(state.rng);
        }

        return true;
    }

    void DrawColumns() {
        for (int i = 0; i < 5; i++) { // 5 columns
            for (int j = 0; j < 4; j++) { // 3 symbols per column + 1 the newly generated one
                constexpr int x_start = 14;
                constexpr int y_start = 102;

                const int symbolID = state.columns[i].symbols[j];
                olc::Decal* decalPointer = assets.GetFromId(symbolID);

                float positionX = static_cast<float>(x_start) + (static_cast<float>(i) * 40.0f);
                float positionY = static_cast<float>(y_start) + (static_cast<float>(j) * 35.0f) + state.columns[i].offset;

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
        olc::Decal* leverDecal = assets.GetFromId(10 + 1 + state.lever.frame);
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
        olc::Decal* icon = assets.GetFromId(iconID);
        if (!icon) return;

        float iconWidth = 16.0f;
        float spacing = 20.0f;

        DrawDecal({ 14, static_cast<float>(y - 12) }, icon, { 0.3f, 0.3f });
        DrawString(static_cast<int>(14.0f + iconWidth + spacing), y, value, color);
    }

    void DrawWinLines() {
        if (IsAnyColumnSpinning()) return;
        for (auto& coord : state.win.chainCoordinates) {

            int column = coord.first;
            int row = coord.second + 1; // cuz 4 images loaded, only 3 seen

            // same calculations as in column
            float positionX = 14.0f + (static_cast<float>(column) * 40.0f);
            float positionY = 102.0f + (static_cast<float>(row) * 35.0f) + state.columns[column].offset;

            FillRectDecal({ positionX, positionY }, { 31, 31 }, olc::Pixel(10, 20, 200, 50));
        }
    }

    bool OnUserUpdate(float elapsedTime) override {

        if (state.coins < 10 && !IsAnyColumnSpinning()) {
            SetDrawTarget(1);
            Clear(olc::BLANK);

            SetDrawTarget(nullptr);
            Clear(olc::WHITE);

            DrawString(53,85, "GAME OVER", olc::RED, 2);
            DrawStringCentered(145, "PRESS R TO RESTART", olc::BLACK);

            if (GetKey(olc::Key::R).bPressed) {
                state.coins = 100;
                state.spins = 0;
                state.currentBet = 10;
                state.currentBetType = 10;
            }
            return true;
        } else {

        SetDrawTarget(1);
        Clear(olc::WHITE);

        placeBetOnPress();
        startSpinningOnPress();
        state.lever.Animate(elapsedTime);

        for (auto& column : state.columns) {
            column.Update(state.rng, elapsedTime);
        }
        startCountingAfterSpin();

        DrawColumns();
        DrawBoundaries();
        DrawLever();
        DrawWinLines();

        DrawStringCentered(10, "PLACE A BET - ARROW KEYS &", olc::BLACK);
        DrawStringCentered(20, "PRESS SPACE TO GAMBLE", olc::BLACK);
        DrawValueWithIcon(60, 21, std::to_string(state.coins), olc::BLACK);
        DrawValueWithIcon(96, 22, std::to_string(state.spins), olc::BLACK);

        DrawString(105, 60, "PLACE YOUR BET", olc::BLACK);

        DrawString(110, 93, std::to_string(state.currentBet), olc::BLACK);
        FillTriangle(117, 80, 117 - 4, 80 + 6, 117 + 4, 80 + 6, olc::BLACK);
        FillTriangle(117, 106 + 6, 117 - 4, 106, 117 + 4, 106, olc::BLACK);

        DrawString(160, 93, "ALL IN", olc::BLACK);

        if (state.currentBetType == 10) {
            FillRectDecal({ 105, 90 }, { 34, 15 }, olc::Pixel(10, 20, 200, 50));
        } else {
            FillRectDecal({ 155, 90 }, { 58, 15 }, olc::Pixel(10, 20, 200, 50));
        }

        return true;
        }
    }
};

int main() {
    UltraGambler game;
    if (game.Construct(250, 250, 2, 2))
        game.Start();
    return 0;
}