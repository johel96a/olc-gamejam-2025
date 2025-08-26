#pragma once

#include "IScene.h"

class GameplayScene : public IScene {
public:
    GameplayScene() = default;
    virtual ~GameplayScene() = default;

    void Update() override {}
    void Render() override {}

private:

};