#pragma once

#include <unordered_map>
#include <memory>

#include "olcPixelGameEngine.h"
#include "olcPGEX_SplashScreen.h"

#include "Animation.h"
#include "IScene.h"

enum class SceneID {
    Splash,
    MainMenu,
    Gameplay,
};

class Application : public olc::PixelGameEngine {
public:
    Application();
    virtual ~Application();

public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    bool OnUserDestroy() override;

private:
    // olc::SplashScreen m_olcSplashScreen;
    std::unordered_map<SceneID, std::unique_ptr<IScene>> m_scenes;

    std::unique_ptr<olc::Sprite> m_playerIdleSpriteSheet;
    std::unique_ptr<olc::Decal> m_playerIdleDecal;
    std::unique_ptr<Animation> m_idleAnimation;

    olc::vf2d m_playerPosition;
    olc::vf2d m_playerVelocity ;
    float m_acceleration;
    float m_deceleration;
    float m_maxSpeed;
};


