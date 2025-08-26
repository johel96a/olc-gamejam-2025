#pragma once

#include "IScene.h"
#include "Player.h"

class GameplayScene : public IScene {
public:
    GameplayScene(olc::PixelGameEngine* pge);
    virtual ~GameplayScene() = default;

    void Update(float fElapsedTime) override;
    void Render() override;

private:
    Player m_player;
};