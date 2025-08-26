#pragma once

#include "olcPixelGameEngine.h"

class IScene {
public:
    IScene(olc::PixelGameEngine* _pge)
        : pge(_pge) {}
    virtual ~IScene() = default;

    virtual void Update(float fElapsedTime) = 0;
    virtual void Render() = 0;

protected:
    olc::PixelGameEngine* pge;
};