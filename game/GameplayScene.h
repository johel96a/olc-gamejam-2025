#pragma once

#include <algorithm>

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
    void checkAccuracy();
    bool isExactEdgePixel(const olc::vi2d& drillPixel, const std::unordered_set<olc::vi2d>& edgeSet);

private:
    GameState m_state;
    Player m_player;

    std::unique_ptr<olc::Sprite> m_testSpr;
    std::unordered_set<olc::vi2d> m_edgePixels;
};