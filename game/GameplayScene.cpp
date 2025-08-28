#include "GameplayScene.h"

GameplayScene::GameplayScene(olc::PixelGameEngine* pge) 
    : IScene(pge)
{
    m_state = GameState::Ready;

    m_player.SetOnDrillCompleteCallback([&]() {
        checkAccuracy();
    });

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

        case GameState::Playing: {
            m_player.Update(pge, fElapsedTime);
     
            if (pge->GetKey(olc::Key::ENTER).bPressed)
            {
                checkAccuracy();
            }

            break;
        }
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
            drawShape();
            m_player.Render(pge);
            break;
    }
}

void GameplayScene::loadShape() 
{
    m_testSpr = std::make_unique<olc::Sprite>();
    if (!m_testSpr->LoadFromFile("assets/textures/shapes/bang_1.png")) {
        std::cerr << "Failed to load misc.png\n";
    }
    const int width = m_testSpr->width;
    const int height = m_testSpr->height;

    const olc::vi2d shapeOffset = {
        (pge->ScreenWidth() - width) / 2,
        (pge->ScreenHeight() - height) / 2
    };

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
                m_edgePixels.insert({ x + shapeOffset.x, y + shapeOffset.y });
            }
        }
    }
}

void GameplayScene::drawShape()
{
    for (const auto& p : m_edgePixels)
    {
        pge->Draw(olc::vi2d(p.x, p.y), olc::Pixel(99, 84, 91));
    } 
}

void GameplayScene::checkAccuracy()
{
    int matched = 0;
    const auto& drilled = m_player.GetDrilledPixels();
    std::unordered_set<olc::vi2d> edgeSet(m_edgePixels.begin(), m_edgePixels.end());

    for (const auto& edgePixel : edgeSet)
    {
        if (drilled.count(edgePixel) > 0)
        {
            matched++;
        }
    }

    int total = static_cast<int>(edgeSet.size());
    float accuracy = total > 0 ? (matched / static_cast<float>(total)) * 100.0f : 0.0f;

    std::cout << "Matched: " << matched << " / " << total 
            << " (" << accuracy << "% accuracy)" << std::endl;
}

bool GameplayScene::isExactEdgePixel(const olc::vi2d& drillPixel, const std::unordered_set<olc::vi2d>& edgeSet) {
    return edgeSet.find(drillPixel) != edgeSet.end();
}