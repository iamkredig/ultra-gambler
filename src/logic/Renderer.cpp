#include "Renderer.h"

void DrawStringCentered(int y, const std::string& text, olc::Pixel color, olc::PixelGameEngine& engine) {
    int textWidth = static_cast<int>(text.length() * 8) ;
    int x = (engine.ScreenWidth() - textWidth) / 2;
    engine.DrawString(x, y, text, color);
}

void DrawValueWithIcon(int y, int iconID, const std::string& value, olc::Pixel color, olc::PixelGameEngine& engine, Assets& assets) {
    olc::Decal* icon = assets.GetFromId(iconID);
    if (!icon) return;

    float iconWidth = 16.0f;
    float spacing = 20.0f;

    engine.DrawDecal({ 14, static_cast<float>(y - 12) }, icon, { 0.3f, 0.3f });
    engine.DrawString(static_cast<int>(14.0f + iconWidth + spacing), y, value, color);
}

void DrawGameOver(olc::PixelGameEngine& engine, GameState& state) {
    engine.SetDrawTarget(1);
    engine.Clear(olc::BLANK);

    engine.SetDrawTarget(nullptr);
    engine.Clear(olc::WHITE);

    engine.DrawString(53,85, "GAME OVER", olc::RED, 2);
    DrawStringCentered(145, "PRESS R TO RESTART", olc::BLACK, engine);
    DrawStringCentered(155, "most coins had: " + std::to_string(state.bestCoins), olc::BLACK, engine);

    if (engine.GetKey(olc::Key::R).bPressed) { // !! AUSLAGERN
        state.coins = 100;
        state.bestCoins = state.coins;
        state.spins = 0;
        state.currentBet = 10;
        state.currentBetType = 10;
    }
}

void DrawColumns(olc::PixelGameEngine& engine, GameState& state, Assets& assets) {
    for (int i = 0; i < 5; i++) { // 5 columns
        for (int j = 0; j < 4; j++) { // 3 symbols per column + 1 the newly generated one
            constexpr int x_start = 14;
            constexpr int y_start = 102;

            const int symbolID = state.columns[i].symbols[j];
            olc::Decal* decalPointer = assets.GetFromId(symbolID);

            float positionX = static_cast<float>(x_start) + (static_cast<float>(i) * 40.0f);
            float positionY = static_cast<float>(y_start) + (static_cast<float>(j) * 35.0f) + state.columns[i].offset;

            if (decalPointer != nullptr) {
                engine.DrawDecal({ positionX, positionY }, decalPointer, { 0.3f, 0.3f });
            }
        }
    }
}

void DrawBoundaries(olc::PixelGameEngine& engine) {
    engine.SetDrawTarget(nullptr);
    engine.Clear(olc::BLANK);
    engine.FillRect({0, 0}, {engine.ScreenWidth(), 137}, olc::WHITE);
    engine.FillRect({0, 240}, {engine.ScreenWidth(), engine.ScreenHeight() - 240}, olc::WHITE);
}

void DrawUI(olc::PixelGameEngine& engine, GameState& state, Assets& assets) {
    DrawStringCentered(10, "PLACE A BET - ARROW KEYS &", olc::BLACK, engine);
    DrawStringCentered(20, "PRESS SPACE TO GAMBLE", olc::BLACK, engine);
    DrawValueWithIcon(60, 21, std::to_string(state.coins), olc::BLACK, engine, assets);
    DrawValueWithIcon(96, 22, std::to_string(state.spins), olc::BLACK, engine, assets);

    engine.DrawString(105, 60, "PLACE YOUR BET", olc::BLACK);

    engine.DrawString(110, 93, std::to_string(state.currentBet), olc::BLACK);
    engine.FillTriangle(117, 80, 117 - 4, 80 + 6, 117 + 4, 80 + 6, olc::BLACK);
    engine.FillTriangle(117, 106 + 6, 117 - 4, 106, 117 + 4, 106, olc::BLACK);

    engine.DrawString(160, 93, "ALL IN", olc::BLACK);

    if (state.currentBetType == 10) {
        engine.FillRectDecal({ 105, 90 }, { 34, 15 }, olc::Pixel(10, 20, 200, 50));
    } else {
        engine.FillRectDecal({ 155, 90 }, { 58, 15 }, olc::Pixel(10, 20, 200, 50));
    }
}

void DrawLever(olc::PixelGameEngine& engine, GameState& state, Assets& assets) {
    olc::Decal* leverDecal = assets.GetFromId(10 + 1 + state.lever.frame);
    if (leverDecal != nullptr) {
        engine.DrawDecal({ 215, 122 }, leverDecal, { 0.3f, 0.3f });
    }
}

void DrawWinLines(olc::PixelGameEngine& engine, GameState& state) {
    if (state.IsAnyColumnSpinning()) return;
    for (auto& coord : state.win.chainCoordinates) {

        int column = coord.first;
        int row = coord.second + 1; // cuz 4 images loaded, only 3 seen

        // same calculations as in column
        float positionX = 14.0f + (static_cast<float>(column) * 40.0f);
        float positionY = 102.0f + (static_cast<float>(row) * 35.0f) + state.columns[column].offset;

        engine.FillRectDecal({ positionX, positionY }, { 31, 31 }, olc::Pixel(10, 20, 200, 50));
    }
}

void Renderer::Draw(olc::PixelGameEngine& engine, GameState& state, Assets& assets) {
    engine.SetDrawTarget(1);
    engine.Clear(olc::WHITE);

    if (state.coins < 10 && !state.IsAnyColumnSpinning()) {
        DrawGameOver(engine, state);
        return;
    }

    DrawColumns(engine, state, assets);
    DrawBoundaries(engine);
    DrawLever(engine, state, assets);
    DrawUI(engine, state, assets);
    DrawWinLines(engine, state);
}