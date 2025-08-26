#pragma once

#include "Animation.h"

enum class Direction { Right, Down, Up, Left };

class Player {
public:
    Player();
    virtual ~Player() = default;

    void Update(olc::PixelGameEngine* pge, float fElapsedTime);
    void Render(olc::PixelGameEngine* pge);
    
private:
    void handleInput(olc::PixelGameEngine* pge);
    
private:
    std::unique_ptr<olc::Sprite> m_playerSpriteSheet;
    std::unique_ptr<olc::Decal> m_playerDecal;
    std::unique_ptr<Animation> m_animation;

    olc::vf2d m_playerPosition;
    olc::vf2d m_playerVelocity;
    olc::vf2d m_input;
    float m_acceleration;
    float m_deceleration;
    float m_maxSpeed;
    Direction m_direction;
    Direction m_previousDirection;
    bool m_flipX;
};