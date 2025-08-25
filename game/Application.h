#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_SplashScreen.h"

#include "Animation.h"

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

    olc::Sprite* m_playerIdleSpriteSheet;
    olc::Decal* m_playerIdleDecal;
    Animation* m_idleAnimation;
};


