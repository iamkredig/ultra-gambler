#include "Assets.h"

void Assets::LoadAsset(int id, const std::string& path) {
    sprites[id] = std::make_unique<olc::Sprite>(path);
    decals[id] = std::make_unique<olc::Decal>(sprites[id].get());
}

void Assets::Load() {
    for (int i = 1; i <= 4; i++) {
        LoadAsset(10+i, "./assets/lever/lever-" + std::to_string(i) + ".png");
        LoadAsset(i, "./assets/symbols/" + std::to_string(i) + ".png");
    }

    LoadAsset(21, "./assets/21.png");
    LoadAsset(22, "./assets/22.png");
}

olc::Decal* Assets::GetFromId(int id) {
    if (decals.contains(id)) return decals[id].get();
    return nullptr;
}