#include "GameplayScene.h"

GameplayScene::GameplayScene(olc::PixelGameEngine* pge) 
    : IScene(pge)
{
    m_state = GameState::Ready;
}

void GameplayScene::Update(float fElapsedTime) 
{ 
    switch (m_state)
    {
        case GameState::Ready:
            if (pge->GetKey(olc::Key::SPACE).bPressed)
            {
                loadShape();
                m_state = GameState::Playing;
            }
            break;

        case GameState::Playing:
            m_player.Update(pge, fElapsedTime);
            break;
    }
}

void GameplayScene::Render()
{
    switch (m_state)
    {
        case GameState::Ready:
            pge->DrawString({ 100, 100 }, "Ready! Press SPACE to start", olc::WHITE, 1);
            break;

        case GameState::Playing:
            m_player.Render(pge);
            drawShape();
            break;
    }
}

void GameplayScene::loadShape() 
{
    m_testSpr = std::make_unique<olc::Sprite>();
    if (!m_testSpr->LoadFromFile("assets/textures/arrow.png")) {
        std::cerr << "Failed to load misc.png\n";
    }
    const int width = m_testSpr->width;
    const int height = m_testSpr->height;

    m_edgePixels.reserve(width * height / 4);

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            olc::Pixel current = m_testSpr->GetPixel(x, y);
            if (current.a == 0)
                continue;

            bool isEdge = false;
            // Check 8 neighbors for transparency (alpha == 0)
            for (int ny = -1; ny <= 1 && !isEdge; ny++)
            {
                for (int nx = -1; nx <= 1 && !isEdge; nx++)
                {
                    if (nx == 0 && ny == 0) continue;
                    olc::Pixel neighbor = m_testSpr->GetPixel(x + nx, y + ny);
                    if (neighbor.a == 0) {
                        isEdge = true;
                    }
                }
            }

            if (isEdge) {
                m_edgePixels.push_back({ x, y });
            }
        }
    }
}

void GameplayScene::drawShape()
{
    for (const auto& p : m_edgePixels)
    {
        pge->Draw(olc::vi2d(p.x, p.y), olc::WHITE);
    } 
}
