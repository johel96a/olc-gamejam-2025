#include "olcPixelGameEngine.h"
#include "olcPGEX_SplashScreen.h"

class Example : public olc::PixelGameEngine
{
private:
    olc::SplashScreen m_olcSplashScreen;

public:
    Example() { sAppName = "olc-gamejam-2025"; }

public:
    bool OnUserCreate() override
    {
        return true;
    }


    bool OnUserUpdate(float fElapsedTime) override
    {
        return true;
    }

private:
    bool OnUserDestroy() override
    {
        return true;
    }
};

int main()
{
    Example demo;
    if (demo.Construct(640, 480, 2, 2))
        demo.Start();
    return 0;
}
