#pragma once

#include "IScene.h"

class MainMenuScene : public IScene {
public:
    MainMenuScene(olc::PixelGameEngine* pge);
    virtual ~MainMenuScene() = default;

    void Update(float fElapsedTime) override;
    void Render() override;

private:

};