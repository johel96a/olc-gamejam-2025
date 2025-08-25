#include "Animation.h"

Animation::Animation(olc::Sprite* sprite, int frameWidth, int frameHeight, int framesPerRow, float frameDuration)
    : m_sprite(sprite), m_frameWidth(frameWidth), m_frameHeight(frameHeight), m_framesPerRow(framesPerRow), m_frameDuration(frameDuration),
      m_currentFrame(0), m_currentRow(0), m_frameTimer(0.0f)
{

}

void Animation::Update(float elapsedTime)
{
    m_frameTimer += elapsedTime;
    if (m_frameTimer >= m_frameDuration)
    {
        m_frameTimer = 0.0f;
        m_currentFrame = (m_currentFrame + 1) % m_framesPerRow;
    }
}

void Animation::Draw(olc::PixelGameEngine* pge, olc::Decal* decal, olc::vi2d position)
{
    olc::vi2d sourcePos = { m_currentFrame * m_frameWidth, m_currentRow * m_frameHeight };
    olc::vi2d sourceSize = { m_frameWidth, m_frameHeight };
    pge->DrawPartialDecal(position, decal, sourcePos, sourceSize);
}

void Animation::SetRow(int row)
{
    if (row != m_currentRow)
    {
        m_currentRow = row;
        Reset();
    }
}

void Animation::Reset()
{
    m_currentFrame = 0;
    m_frameTimer = 0.0f;
}
