// Application.cpp
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
*/

#include "Application.h"

#include "Utility.h"

#include <Windows.h>

namespace VTFViewer {

    Application::Application()
        :m_Running(true)
    {
        LOG("Application constructor");
    }

    Application::~Application()
    {
        LOG("Application deconstructor");
    }

    void Application::Run(int argc, char* argv[])
    {
        while (m_Running)
        {
            if (GetAsyncKeyState(VK_ESCAPE) & 1)
                Shutdown();
        }
    }

    void Application::Shutdown()
    {
        LOG("Shutting down application");
        m_Running = false;
    }

}