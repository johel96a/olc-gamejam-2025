#pragma once

#include "Animation.h"

class Player {
public:
    Player();
    virtual ~Player() = default;

    void Update(olc::PixelGameEngine* pge, float fElapsedTime);
    void Render(olc::PixelGameEngine* pge);
    
private:
    void handleInput(olc::PixelGameEngine* pge);
    
private:
    std::unique_ptr<olc::Sprite> m_playerIdleSpriteSheet;
    std::unique_ptr<olc::Decal> m_playerIdleDecal;
    std::unique_ptr<Animation> m_idleAnimation;

    olc::vf2d m_playerPosition;
    olc::vf2d m_playerVelocity;
    olc::vf2d m_input;
    float m_acceleration;
    float m_deceleration;
    float m_maxSpeed;
};