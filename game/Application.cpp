#include "Application.h"

#include "Animation.h"

Application::Application()
{
    // Pick-a-Shape ?
    sAppName = "olc-gamejam-2025";
}

Application::~Application()
{
}

bool Application::OnUserCreate()
{
    m_playerIdleSpriteSheet = new olc::Sprite();
    if (!m_playerIdleSpriteSheet->LoadFromFile("assets/player_idle.png")) {
        return false;
    }
        
    m_playerIdleDecal = new olc::Decal(m_playerIdleSpriteSheet);

    m_idleAnimation = new Animation(m_playerIdleSpriteSheet, 80, 80, 4, 0.15f);

    return true;
}

bool Application::OnUserUpdate(float fElapsedTime)
{
    m_idleAnimation->Update(fElapsedTime);

    Clear(olc::BLACK);

    m_idleAnimation->Draw(this, m_playerIdleDecal, { 100, 100 });

    return true;
}

bool Application::OnUserDestroy()
{
    delete m_playerIdleSpriteSheet;
    delete m_idleAnimation;

    return true;
}
