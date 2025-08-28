#include "Player.h"

Player::Player()
{
    loadParts();

    m_input = { 0.0f, 1.0f };
    m_playerPosition = { 200.f, 150.f };
    m_playerVelocity = { 0.0f, 0.0f };
    m_acceleration = 600.0f;
    m_deceleration = 1000.0f;
    m_maxSpeed = 20.0f;
    m_flipX = false;
    m_drillCooldown = 0.0f;
}

void Player::loadParts() 
{
    m_spriteInfo = AssetManager::Get().LoadSpriteSheet("assets/assets.json", "character_body");

    m_bodySprite = std::make_unique<olc::Sprite>();
    if (!m_bodySprite->LoadFromFile(m_spriteInfo.path)) {
        std::cerr << "Failed to load sprite sheet: " << m_spriteInfo.path << std::endl;
    }

    m_bodyDecal = std::make_unique<olc::Decal>(m_bodySprite.get());

    m_animation = std::make_unique<Animation>(
        m_bodySprite.get(),
        m_spriteInfo.sprite_width,
        m_spriteInfo.sprite_height,
        m_spriteInfo.columns,
        m_spriteInfo.default_animation_speed
    );

    m_animation->SetRow(2);
}

void Player::handleInput(olc::PixelGameEngine* pge, float fElapsedTime) 
{
    olc::vf2d targetInput = { 0.0f, 0.0f };

    if (pge->GetKey(olc::Key::W).bHeld || pge->GetKey(olc::Key::UP).bHeld)    targetInput.y -= 1.0f;
    if (pge->GetKey(olc::Key::S).bHeld || pge->GetKey(olc::Key::DOWN).bHeld)  targetInput.y += 1.0f;
    if (pge->GetKey(olc::Key::A).bHeld || pge->GetKey(olc::Key::LEFT).bHeld)  targetInput.x -= 1.0f;
    if (pge->GetKey(olc::Key::D).bHeld || pge->GetKey(olc::Key::RIGHT).bHeld) targetInput.x += 1.0f;

    if (targetInput.mag2() > 0.0f) {
        targetInput = targetInput.norm();
    }

    constexpr float smoothing = 10.0f;
    m_input += (targetInput - m_input) * smoothing * fElapsedTime;

    if (m_input.mag2() > 1.0f) {
        m_input = m_input.norm();
    }
}

void Player::Update(olc::PixelGameEngine* pge, float fElapsedTime)
{
    handleInput(pge, fElapsedTime);

    m_input = m_input.norm();
    m_playerVelocity = m_input * m_maxSpeed;
    m_playerPosition += m_playerVelocity * fElapsedTime;

    m_animation->Update(fElapsedTime);

    const olc::vi2d drillCenter = {
        static_cast<int>(std::round(m_playerPosition.x)),
        static_cast<int>(std::round(m_playerPosition.y)) + 10
    };

    if (m_isDrilling) 
    {
        m_drillCooldown -= fElapsedTime;

        if (m_drillCooldown <= 0.0f) 
        {          
            for (int dx = -1; dx <= 1; dx++) 
            {
                for (int dy = -1; dy <= 1; dy++) 
                {
                    m_drilledPixels.insert(drillCenter + olc::vi2d(dx, dy));
                }
            }

            m_drillCooldown = 0.05f; // 50 ms
        }
    }

    // Loop detection and store the new pixels AFTER loop check ???

}



void Player::Render(olc::PixelGameEngine* pge)
{
    const olc::vf2d drawPos = m_playerPosition - olc::vf2d(m_spriteInfo.sprite_width / 2.0f, m_spriteInfo.sprite_height / 2.0f);
    m_animation->Draw(pge, m_bodyDecal.get(), drawPos, m_flipX);

    const olc::vi2d drillMarker = {
        static_cast<int>(std::round(m_playerPosition.x)),
        static_cast<int>(std::round(m_playerPosition.y)) + 10
    };

    for (const auto& p : m_drilledPixels) {
        pge->Draw(p, olc::Pixel(82, 61, 60));
    }
}

void Player::ToggleDrill(bool status) 
{
    m_isDrilling = status;
}

const std::unordered_set<olc::vi2d>& Player::GetDrilledPixels() const 
{
    return m_drilledPixels;
}
