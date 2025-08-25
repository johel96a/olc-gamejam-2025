#pragma once

#include "Application.h"

int main()
{
    Application app;
    if (app.Construct(640, 480, 2, 2))
        app.Start();
    return 0;
}