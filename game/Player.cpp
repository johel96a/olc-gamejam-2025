#include "Player.h"
#include "AssetManager.h"

Player::Player() 
{
    // Load player sprite sheet info from asset manager
    const SpriteSheetInfo& playerInfo = AssetManager::Get().LoadSpriteSheet("assets/assets.json", "player_spritesheet");

    m_playerSpriteSheet = std::make_unique<olc::Sprite>();
    if (!m_playerSpriteSheet->LoadFromFile(playerInfo.path)) {
        std::cerr << "Failed to load sprite sheet: " << playerInfo.path << std::endl;
    }

    m_playerDecal = std::make_unique<olc::Decal>(m_playerSpriteSheet.get());

    m_animation = std::make_unique<Animation>(
        m_playerSpriteSheet.get(),
        playerInfo.sprite_width,
        playerInfo.sprite_height,
        playerInfo.columns,
        playerInfo.default_animation_speed
    );

    m_playerPosition = { 200.f, 150.f };
    m_playerVelocity = { 0.0f, 0.0f };
    m_acceleration = 600.0f;
    m_deceleration = 1000.0f;
    m_maxSpeed = 20.0f;
    m_direction = Direction::Right;
    m_previousDirection = m_direction;
    m_flipX = false;
}

void Player::handleInput(olc::PixelGameEngine* pge, float fElapsedTime) 
{
    olc::vf2d targetInput = {0.0f, 0.0f};

    if (pge->GetKey(olc::Key::W).bHeld || pge->GetKey(olc::Key::UP).bHeld)    targetInput.y -= 1.0f;
    if (pge->GetKey(olc::Key::S).bHeld || pge->GetKey(olc::Key::DOWN).bHeld)  targetInput.y += 1.0f;
    if (pge->GetKey(olc::Key::A).bHeld || pge->GetKey(olc::Key::LEFT).bHeld)  targetInput.x -= 1.0f;
    if (pge->GetKey(olc::Key::D).bHeld || pge->GetKey(olc::Key::RIGHT).bHeld) targetInput.x += 1.0f;

    if (targetInput.mag2() > 0.0f)
        targetInput = targetInput.norm();

    // lerp
    float smoothing = 10.0f; // Higher = faster interpolation, tweak to feel right
    m_input += (targetInput - m_input) * smoothing * fElapsedTime;

    if (m_input.mag2() > 1.0f) {
        m_input = m_input.norm();
    }
}

void Player::updateDirection() 
{
    Direction newDirection = m_previousDirection;

    if (m_input.y < 0) newDirection = Direction::Up;
    else if (m_input.y > 0) newDirection = Direction::Down;
    else if (m_input.x > 0) newDirection = Direction::Right;
    else if (m_input.x < 0) newDirection = Direction::Left;
    // else keep previous if no input

    m_flipX = (newDirection == Direction::Left);

    // Left to Right animation row since we flip the decal
    int animationRow = (newDirection == Direction::Left) ? static_cast<int>(Direction::Right) : static_cast<int>(newDirection);

    if (newDirection != m_previousDirection) {
        m_animation->SetRow(animationRow);
        m_previousDirection = newDirection;
    }

    m_direction = newDirection;
}

void Player::Update(olc::PixelGameEngine* pge, float fElapsedTime)
{
    handleInput(pge, fElapsedTime);

    if (m_input.mag2() > 0.0f)
    {
        m_input = m_input.norm();
        m_playerVelocity = m_input * m_maxSpeed;
    }
    else
    {
        m_playerVelocity = { 0.0f, 0.0f };
    }

    m_playerPosition += m_playerVelocity * fElapsedTime;

    updateDirection();

    m_animation->Update(fElapsedTime);


    float m_drillCooldown = 0.0f;

    if (m_isDrilling) 
    {
        m_drillCooldown -= fElapsedTime;
        if (m_drillCooldown <= 0.0f) 
        {
            olc::vi2d drillCenter = {
                static_cast<int>(std::round(m_playerPosition.x)),
                static_cast<int>(std::round(m_playerPosition.y)) + 10
            };

            for (int dx = -1; dx <= 1; dx++) 
            {
                for (int dy = -1; dy <= 1; dy++) 
                {
                    olc::vi2d drillPixel = drillCenter + olc::vi2d(dx, dy);
                    m_drilledPixels.insert(drillPixel);
                }
            }

            m_drillCooldown = 0.05f; // 50 ms between drill points
        }
    }
}

void Player::Render(olc::PixelGameEngine* pge)
{
    olc::vi2d mouseScreenPos = pge->GetMousePos();
    olc::vf2d mouseWorldPos = olc::vf2d(mouseScreenPos);

    olc::vf2d dir = mouseWorldPos - m_playerPosition;

    if (dir.mag() == 0) {
        dir = { 1.0f, 0.0f };
    }

    olc::vf2d markerPos = m_playerPosition + olc::vi2d(0, 10);
    pge->Draw(markerPos, olc::RED);

    // TODO: all sprites are not 80, 80
    // Center sprite based on 80x80 size
    olc::vf2d drawPos = m_playerPosition - olc::vf2d(40.0f, 40.0f);
    m_animation->Draw(pge, m_playerDecal.get(), drawPos, m_flipX);

    for (const auto& p : m_drilledPixels) {
        pge->Draw(p, olc::RED);
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
