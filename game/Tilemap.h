#pragma once

#include "olcPixelGameEngine.h"

#include "AssetManager.h"

enum class TileType : int {
    GROUND_C = 0, GROUND_U, GROUND_D, GROUND_L, GROUND_R,
    GROUND_UL, GROUND_UR, GROUND_DL, GROUND_DR,

    PATH_C, PATH_U, PATH_D, PATH_L, PATH_R,
    PATH_UL, PATH_UR, PATH_DL, PATH_DR,
};

class TileMap {
public:
    TileMap();

    void Print();

private:
    const int MAP_WIDTH = 18;
    const int MAP_HEIGHT = 15;
    std::vector<std::vector<TileType>> m_tiles;

    std::unique_ptr<olc::Sprite> m_groundSprite;
    std::unique_ptr<olc::Sprite> m_pathSprite;

    SpriteSheetInfo m_groundSpriteInfo;
    SpriteSheetInfo m_pathSpriteInfo;
};