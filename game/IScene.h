#pragma once

class IScene {
public:
    virtual ~IScene() = default;

    virtual void Update() = 0;
    virtual void Render() = 0;

private:

};