#pragma once

#include "olcPixelGameEngine.h"

class Animation {
public:
    Animation(olc::Sprite* sprite, int frameWidth, int frameHeight, int framesPerRow, float frameDuration);
    virtual ~Animation() = default;

    void Update(float elapsedTime);
    void Draw(olc::PixelGameEngine* pge, olc::Decal* decal, olc::vi2d position);
    void SetRow(int row);
    void Reset();

private:
    olc::Sprite* m_sprite;

    int m_frameWidth;
    int m_frameHeight;
    int m_framesPerRow;
    float m_frameDuration;

    int m_currentFrame;
    int m_currentRow;
    float m_frameTimer;
};