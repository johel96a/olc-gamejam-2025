#include "Tilemap.h"

TileMap::TileMap() 
    : m_tiles(MAP_HEIGHT, std::vector<TileType>(MAP_WIDTH, TileType::GROUND_C))
{
    // Set ground area (11x11)
    int groundStartX = (MAP_WIDTH - 11) / 2;
    int groundStartY = (MAP_HEIGHT - 11) / 2;

    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 11; x++) {
            // Determine if corner
            bool top = (y == 0);
            bool bottom = (y == 10);
            bool left = (x == 0);
            bool right = (x == 10);

            if (top && left)
                m_tiles[groundStartY + y][groundStartX + x] = TileType::GROUND_UL;
            else if (top && right)
                m_tiles[groundStartY + y][groundStartX + x] = TileType::GROUND_UR;
            else if (bottom && left)
                m_tiles[groundStartY + y][groundStartX + x] = TileType::GROUND_DL;
            else if (bottom && right)
                m_tiles[groundStartY + y][groundStartX + x] = TileType::GROUND_DR;
            else if (top)
                m_tiles[groundStartY + y][groundStartX + x] = TileType::GROUND_U;
            else if (bottom)
                m_tiles[groundStartY + y][groundStartX + x] = TileType::GROUND_D;
            else if (left)
                m_tiles[groundStartY + y][groundStartX + x] = TileType::GROUND_L;
            else if (right)
                m_tiles[groundStartY + y][groundStartX + x] = TileType::GROUND_R;
            else
                m_tiles[groundStartY + y][groundStartX + x] = TileType::GROUND_C;
        }
    }

    // Set path area (9x9), inside ground
    int pathStartX = groundStartX + 1;
    int pathStartY = groundStartY + 1;

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            bool top = (y == 0);
            bool bottom = (y == 8);
            bool left = (x == 0);
            bool right = (x == 8);

            if (top && left)
                m_tiles[pathStartY + y][pathStartX + x] = TileType::PATH_UL;
            else if (top && right)
                m_tiles[pathStartY + y][pathStartX + x] = TileType::PATH_UR;
            else if (bottom && left)
                m_tiles[pathStartY + y][pathStartX + x] = TileType::PATH_DL;
            else if (bottom && right)
                m_tiles[pathStartY + y][pathStartX + x] = TileType::PATH_DR;
            else if (top)
                m_tiles[pathStartY + y][pathStartX + x] = TileType::PATH_U;
            else if (bottom)
                m_tiles[pathStartY + y][pathStartX + x] = TileType::PATH_D;
            else if (left)
                m_tiles[pathStartY + y][pathStartX + x] = TileType::PATH_L;
            else if (right)
                m_tiles[pathStartY + y][pathStartX + x] = TileType::PATH_R;
            else
                m_tiles[pathStartY + y][pathStartX + x] = TileType::PATH_C; // Center tile
        }
    }
}

void TileMap::Print() 
{
    for (const auto& row : m_tiles) {
        for (const auto& tile : row) {
            std::cout << static_cast<int>(tile);
        }
        std::cout << '\n';
    }
}
