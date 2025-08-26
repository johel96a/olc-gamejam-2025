#include "GameplayScene.h"

GameplayScene::GameplayScene(olc::PixelGameEngine* pge) 
    : IScene(pge)
{

}

void GameplayScene::Update(float fElapsedTime) 
{ 
    m_player.Update(pge, fElapsedTime); 
}

void GameplayScene::Render()
{
    m_player.Render(pge);
}
