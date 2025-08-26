#pragma once

#include "IScene.h"
#include "Player.h"

enum class GameState {
    Ready,
    Playing
};

class GameplayScene : public IScene {
public:
    GameplayScene(olc::PixelGameEngine* pge);
    virtual ~GameplayScene() = default;

    void Update(float fElapsedTime) override;
    void Render() override;

private:
    // TODO: refactor
    void loadShape();
    void drawShape();

private:
    GameState m_state;

    Player m_player;

    std::unique_ptr<olc::Sprite> m_testSpr;
    std::vector<olc::vi2d> m_edgePixels;
};