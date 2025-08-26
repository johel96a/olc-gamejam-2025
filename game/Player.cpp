#include "Player.h"

#include "AssetManager.h"

Player::Player() 
{
    const SpriteSheetInfo& playerInfo = AssetManager::Get().LoadSpriteSheet("assets/assets.json", "player_spritesheet");

    m_playerSpriteSheet = std::make_unique<olc::Sprite>();
    if (!m_playerSpriteSheet->LoadFromFile(playerInfo.path)) {
        std::cerr << "Failed to load: " << std::endl;
    }
    std::cout << playerInfo.path;
    m_playerDecal = std::make_unique<olc::Decal>(m_playerSpriteSheet.get());

    m_animation = std::make_unique<Animation>(
        m_playerSpriteSheet.get(),
        playerInfo.sprite_width,
        playerInfo.sprite_height,
        playerInfo.columns,    
        0.15f
    );

    m_playerPosition = { 100.f, 100.f };
    m_playerVelocity = { 0.0f, 0.0f };
    m_acceleration = 600.0f;
    m_deceleration = 1000.0f;
    m_maxSpeed = 50.0f;
}

void Player::handleInput(olc::PixelGameEngine* pge) 
{
    m_input = { 0.0f, 0.0f };

    if (pge->GetKey(olc::Key::W).bHeld || pge->GetKey(olc::Key::UP).bHeld)    m_input.y -= 1.0f;
    if (pge->GetKey(olc::Key::S).bHeld || pge->GetKey(olc::Key::DOWN).bHeld)  m_input.y += 1.0f;
    if (pge->GetKey(olc::Key::A).bHeld || pge->GetKey(olc::Key::LEFT).bHeld)  m_input.x -= 1.0f;
    if (pge->GetKey(olc::Key::D).bHeld || pge->GetKey(olc::Key::RIGHT).bHeld) m_input.x += 1.0f;
}

void Player::Update(olc::PixelGameEngine* pge, float fElapsedTime)
{
    handleInput(pge);

    if (m_input.mag2() > 0.0f)
    {
        m_input = m_input.norm();
        m_playerVelocity += m_input * m_acceleration * fElapsedTime;
     
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

    m_animation->Update(fElapsedTime);
} 

void Player::Render(olc::PixelGameEngine* pge)
{
    m_animation->Draw(pge, m_playerDecal.get(), m_playerPosition);
} 