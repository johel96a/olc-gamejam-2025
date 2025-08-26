#pragma once

#include <unordered_map>
#include <memory>

#include "olcPixelGameEngine.h"
#include "olcPGEX_SplashScreen.h"

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
    
private:
    // olc::SplashScreen m_olcSplashScreen;
    std::unordered_map<SceneID, std::unique_ptr<IScene>> m_scenes;

    SceneID m_currentScene;
};


