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

void Animation::Draw(olc::PixelGameEngine* pge, olc::Decal* decal, olc::vi2d position, bool flipX)
{
    olc::vi2d sourcePos = { m_currentFrame * m_frameWidth, m_currentRow * m_frameHeight };
    olc::vi2d sourceSize = { m_frameWidth, m_frameHeight };
    olc::vf2d scale = flipX ? olc::vf2d(-1.0f, 1.0f) : olc::vf2d(1.0f, 1.0f);
    olc::vf2d drawPos = flipX ? olc::vf2d(position.x + m_frameWidth, position.y) : olc::vf2d(position.x, position.y);
    pge->DrawPartialDecal(drawPos, decal, sourcePos, sourceSize, scale);
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
