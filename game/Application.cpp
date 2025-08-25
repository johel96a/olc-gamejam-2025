#include "Application.h"

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
    return true;
}

bool Application::OnUserUpdate(float fElapsedTime)
{
    return true;
}

bool Application::OnUserDestroy()
{
    return true;
}
