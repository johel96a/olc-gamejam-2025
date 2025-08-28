#include "Application.h"

#include "AssetManager.h"

#include "MainMenuScene.h"
#include "GameplayScene.h"

Application::Application()
{
    // Pick-a-Shape ?
    sAppName = "olc-gamejam-2025";
}

Application::~Application()
{
}

bool Application::OnUserCreate()
{
    m_scenes[SceneID::MainMenu] = std::make_unique<MainMenuScene>(this);
    m_scenes[SceneID::Gameplay] = std::make_unique<GameplayScene>(this);

    m_currentScene = SceneID::Gameplay;

    return true;
}

bool Application::OnUserUpdate(float fElapsedTime)
{
    m_scenes[m_currentScene]->Update(fElapsedTime);
    
    Clear(olc::BLACK);

    m_scenes[m_currentScene]->Render();

    return true;
}
