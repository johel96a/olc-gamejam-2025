#include "Application.h"

#include "Animation.h"

#include "MainMenuScene.h"
#include "GameplayScene.h"

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
    m_scenes[SceneID::MainMenu] = std::make_unique<MainMenuScene>();
    m_scenes[SceneID::Gameplay] = std::make_unique<GameplayScene>();

    m_playerIdleSpriteSheet = std::make_unique<olc::Sprite>();
    if (!m_playerIdleSpriteSheet->LoadFromFile("assets/player_idle.png")) {
        return false;
    }
        
    m_playerIdleDecal = std::make_unique<olc::Decal>(m_playerIdleSpriteSheet.get());

    m_idleAnimation = std::make_unique<Animation>(m_playerIdleSpriteSheet.get(), 80, 80, 4, 0.15f);

    m_playerPosition = {100.f, 100.f};
    m_playerVelocity = { 0.0f, 0.0f };
    m_acceleration = 600.0f;
    m_deceleration = 1000.0f;
    m_maxSpeed = 50.0f;

    return true;
}

bool Application::OnUserUpdate(float fElapsedTime)
{
    olc::vf2d input = { 0.0f, 0.0f };

    if (GetKey(olc::Key::W).bHeld || GetKey(olc::Key::UP).bHeld)    input.y -= 1.0f;
    if (GetKey(olc::Key::S).bHeld || GetKey(olc::Key::DOWN).bHeld)  input.y += 1.0f;
    if (GetKey(olc::Key::A).bHeld || GetKey(olc::Key::LEFT).bHeld)  input.x -= 1.0f;
    if (GetKey(olc::Key::D).bHeld || GetKey(olc::Key::RIGHT).bHeld) input.x += 1.0f;

    if (input.mag2() > 0.0f)
    {
        input = input.norm();
        m_playerVelocity += input * m_acceleration * fElapsedTime;
     
        if (m_playerVelocity.mag() > m_maxSpeed)
            m_playerVelocity = m_playerVelocity.norm() * m_maxSpeed;
    }
    else
    {
        if (m_playerVelocity.mag2() > 0.0f)
        {
            olc::vf2d decelDir = -m_playerVelocity.norm();
            m_playerVelocity += decelDir * m_deceleration * fElapsedTime;

            if (m_playerVelocity.mag2() < 5.0f)
                m_playerVelocity = { 0.0f, 0.0f };
        }
    }

    m_playerPosition += m_playerVelocity * fElapsedTime;

    m_idleAnimation->Update(fElapsedTime);

    Clear(olc::BLACK);

    m_idleAnimation->Draw(this, m_playerIdleDecal.get(), m_playerPosition);

    return true;
}

bool Application::OnUserDestroy()
{
    return true;
}
