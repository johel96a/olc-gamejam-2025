#pragma once

#include <unordered_set>
#include <functional>

#include "AssetManager.h"
#include "Animation.h"

namespace std {
    template <>
    struct hash<olc::vi2d> {
        size_t operator()(const olc::vi2d& v) const noexcept {
            size_t h1 = std::hash<int>()(v.x);
            size_t h2 = std::hash<int>()(v.y);
            return h1 ^ (h2 << 1);
        }
    };
}

class Player {
public:
    Player();
    virtual ~Player() = default;

    void Update(olc::PixelGameEngine* pge, float fElapsedTime);
    void Render(olc::PixelGameEngine* pge);

    void ToggleDrill(bool status);

    const std::unordered_set<olc::vi2d>& GetDrilledPixels() const;

private:
    void handleInput(olc::PixelGameEngine* pge, float fElapsedTime);
    void updateDirection();
    void loadParts();

private:
    std::unordered_set<olc::vi2d> m_drilledPixels;

    std::unique_ptr<olc::Sprite> m_bodySprite;

    std::unique_ptr<olc::Decal> m_bodyDecal;

    std::unique_ptr<Animation> m_animation;
    SpriteSheetInfo m_spriteInfo;

    olc::vf2d m_playerPosition;
    olc::vf2d m_playerVelocity;

    olc::vf2d m_input;

    float m_acceleration;
    float m_deceleration;
    float m_maxSpeed;

    bool m_flipX;
    bool m_isDrilling;
    float m_drillCooldown;
};