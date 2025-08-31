#include "Player.h"

Player::Player()
{
    loadParts();

    m_input = { 1.0f, 0.0f };
    m_playerPosition = { 200.f, 150.f };
    m_playerVelocity = { 0.0f, 0.0f };
    m_acceleration = 600.0f;
    m_deceleration = 1000.0f;
    m_maxSpeed = 20.0f;
    m_flipX = false;
    m_drillCooldown = 0.0f;
    m_collectingInitialPath = false;

    m_initialDrillPos = { 0, 0 };
    m_drillTimer = 0.0f;

    m_drillGraceTimer = 0.0f;
    m_loopDetectionEnabled = false;
    m_startedInitialPath = false;
    m_startedInitialDrill = false;
    m_collectingInitialPath = false;
}

void Player::loadParts() 
{
    auto maybeSheet = AssetManager::Get().LoadSpriteSheet("assets/assets.json", "character_body");

    if (maybeSheet) {
        m_spriteInfo = *maybeSheet;  // assign actual SpriteSheetInfo
    }
    else {
        std::cerr << "Failed to load character_body sprite sheet\n";
    }

    m_bodySprite = std::make_unique<olc::Sprite>();
    if (!m_bodySprite->LoadFromFile(m_spriteInfo.path)) {
        std::cerr << "Failed to load sprite sheet " << m_spriteInfo.path << std::endl;
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

olc::vi2d Player::GetDrillCenter() const
{
    return {
        static_cast<int>(std::round(m_playerPosition.x)),
        static_cast<int>(std::round(m_playerPosition.y)) + 10
    };
}

void Player::DrillAt(const olc::vi2d& center, std::unordered_set<olc::vi2d>& drillData)
{
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            olc::vi2d p = center + olc::vi2d(dx, dy);
            drillData.insert(p);
        }
    }
}

bool Player::CheckStartingPointReached(const olc::vi2d& drillCenter)
{
    return m_initialDrillPoints.count(drillCenter);
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

void Player::Update(olc::PixelGameEngine* pge, float fElapsedTime, const std::unordered_set<olc::vi2d>& shapeEdgePixels)
{
    m_animation->Update(fElapsedTime);
    
    if (m_isDrilling)
    {
        UpdateMovement(pge, fElapsedTime);
        UpdateDrilling(fElapsedTime, shapeEdgePixels);
    }
    else {
        ResetDrillState();
    }
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
            pge->Draw(p, olc::Pixel(75, 55, 50));
        }

        for (const auto& p : m_initialDrillPoints)
            pge->Draw(p, olc::DARK_YELLOW);
}

void Player::UpdateMovement(olc::PixelGameEngine* pge, float fElapsedTime)
{
    handleInput(pge, fElapsedTime);
    m_input = m_input.norm();
    m_playerVelocity = m_input * m_maxSpeed;
    m_playerPosition += m_playerVelocity * fElapsedTime;
}

void Player::UpdateDrilling(float fElapsedTime, const std::unordered_set<olc::vi2d>& shapeEdgePixels)
{
    m_drillTimer += fElapsedTime;
    m_drillCooldown -= fElapsedTime;

    const olc::vi2d drillCenter = GetDrillCenter();
    const bool currentlyOnShapeEdge = IsOnShapeEdge(shapeEdgePixels);

    // Step 1: Start collecting ONCE if we hit the edge for the first time
    if (currentlyOnShapeEdge && !m_startedInitialPath)
    {
        m_startedInitialPath = true;
        m_collectingInitialPath = true;
        m_drillTimer = 0.0f;
        std::cout << "Started collecting initial path\n";
    }

    // Step 2: Continue collecting for 1 second
    if (m_collectingInitialPath)
    {
        DrillAt(drillCenter, m_initialDrillPoints);

        if (m_drillTimer >= 1.0f)
        {
            m_collectingInitialPath = false;
            m_drillTimer = 0.0f;
            std::cout << "Finished collecting initial path\n";
        }
    }
    else if (m_startedInitialPath && m_drillTimer >= 3.0f)
    {
        // Step 3: After delay, check for returning to initial path
        if (m_initialDrillPoints.count(drillCenter) > 0)
        {
            if(m_onDrillCompleteCallback)
                m_onDrillCompleteCallback();

            m_isDrilling = false;
        }
    }

    // Step 4: Drill area
    if (m_drillCooldown <= 0.0f)
    {
        DrillAt(drillCenter, m_drilledPixels);
        m_drillCooldown = 0.05f;
    }
}

void Player::ResetDrillState()
{
    m_drillTimer = 0.0f;
    m_drillCooldown = 0.0f;
    m_collectingInitialPath = false;
    m_startedInitialPath = false;
    m_initialDrillPoints.clear();
}

void Player::Reset()
{
    m_input = { 1.0f, 0.0f };
    m_playerPosition = { 200.f, 150.f };
    m_playerVelocity = { 0.0f, 0.0f };
    m_acceleration = 600.0f;
    m_deceleration = 1000.0f;
    m_maxSpeed = 20.0f;
    m_flipX = false;
    m_drillCooldown = 0.0f;
    m_collectingInitialPath = false;

    m_initialDrillPos = { 0, 0 };
    m_drillTimer = 0.0f;

    m_drillGraceTimer = 0.0f;
    m_isDrilling = true;
    m_loopDetectionEnabled = false;
    m_startedInitialPath = false;
    m_startedInitialDrill = false;
    m_collectingInitialPath = false;

    if (m_animation)
    {
        m_animation->Reset();
    }

    m_initialDrillPoints.clear();
    m_drilledPixels.clear();
}

bool Player::IsOnShapeEdge(const std::unordered_set<olc::vi2d>& edgePixels) {
    const olc::vi2d drillCenter = {
        static_cast<int>(std::round(m_playerPosition.x)),
        static_cast<int>(std::round(m_playerPosition.y)) + 10
    };

    return edgePixels.count(drillCenter) > 0;
}

void Player::ToggleDrill(bool status) 
{
    m_isDrilling = status;
}

const std::unordered_set<olc::vi2d>& Player::GetDrilledPixels() const 
{
    return m_drilledPixels;
}

void Player::SetPosition(const olc::vf2d& pos)
{
    m_playerPosition = pos;
}
