#pragma once

#include <unordered_set>
#include <functional>

#include "Animation.h"

enum class Direction { Right, Down, Up, Left };

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

private:
    std::unordered_set<olc::vi2d> m_drilledPixels;

    std::unique_ptr<olc::Sprite> m_playerSpriteSheet;
    std::unique_ptr<olc::Decal> m_playerDecal;
    std::unique_ptr<Animation> m_animation;

    olc::vf2d m_playerPosition;
    olc::vf2d m_playerVelocity;
    olc::vf2d m_input;
    float m_acceleration;
    float m_deceleration;
    float m_maxSpeed;
    Direction m_direction;
    olc::vf2d m_moveDirection;
    Direction m_previousDirection;
    bool m_flipX;
    bool m_isDrilling;
};