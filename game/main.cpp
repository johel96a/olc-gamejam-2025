#pragma once

#include "Application.h"

int main()
{
    Application app;
    if (app.Construct(400, 300, 2, 2))
        app.Start();
    return 0;
}