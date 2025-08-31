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

    void Update(olc::PixelGameEngine* pge, float fElapsedTime, const std::unordered_set<olc::vi2d>& shapeEdgePixels);
    void Render(olc::PixelGameEngine* pge);
    void ToggleDrill(bool status);
    bool IsOnShapeEdge(const std::unordered_set<olc::vi2d>& edgePixels);
    const std::unordered_set<olc::vi2d>& GetDrilledPixels() const;

    void SetPosition(const olc::vf2d& pos);
    void Reset();
    
    void SetOnDrillCompleteCallback(std::function<void()> callback) {
        m_onDrillCompleteCallback = std::move(callback);
    }

private:
    void handleInput(olc::PixelGameEngine* pge, float fElapsedTime);
    void updateDirection();
    void loadParts();
    olc::vi2d GetDrillCenter() const;
    void DrillAt(const olc::vi2d& center, std::unordered_set<olc::vi2d>& drillData);
    bool CheckStartingPointReached(const olc::vi2d& drillCenter);
    void UpdateMovement(olc::PixelGameEngine* pge, float fElapsedTime);   
    void UpdateDrilling(float fElapsedTime, const std::unordered_set<olc::vi2d>& shapeEdgePixels);
    void ResetDrillState();


private:
    std::function<void()> m_onDrillCompleteCallback;

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
    bool m_wasDrillingLastFrame;
    bool m_flipX;
    bool m_isDrilling;
    float m_drillCooldown;
    float m_drillActiveTime;
    bool m_hasStartedLoopCheck;
    olc::vi2d m_initialDrillPos = { 0, 0 };

    float m_drillTimer = 0.0f;
    std::unordered_set<olc::vi2d> m_initialDrillPoints;

    float m_drillGraceTimer = 0.0f;
    bool m_loopDetectionEnabled = false;
    bool m_startedInitialPath = false;
    bool m_startedInitialDrill = false;
    bool m_collectingInitialPath = false;
};