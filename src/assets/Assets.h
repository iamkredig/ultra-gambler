#ifndef ULTRA_GAMBLER_ASSETS_H
#define ULTRA_GAMBLER_ASSETS_H

#pragma once
#include "olcPixelGameEngine.h"
#include <map>
#include <memory>

class Assets {
public:
    void Load();
    olc::Decal* GetFromId(int id);

private:
    // id (0-4) and sprite/decal
    std::map<int, std::unique_ptr<olc::Sprite>> sprites;
    std::map<int, std::unique_ptr<olc::Decal>> decals;

    void LoadAsset(int id, const std::string& path);
};

#endif