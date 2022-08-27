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
        :m_Running(true), m_MainWindow(nullptr)
    {
        LOG("Application constructor");
        m_MainWindow = new MainWindow("VTF Viewer", 300, 500);
    }

    Application::~Application()
    {
        LOG("Application deconstructor");
        delete m_MainWindow;
    }

    void Application::Run(int argc, char* argv[])
    {
        while (!m_MainWindow->ShouldClose() && m_Running)
        {
            if (GetAsyncKeyState(VK_ESCAPE) & 1)
                Shutdown();

            m_MainWindow->Render();
            m_MainWindow->PollInput();
        }
    }

    void Application::Shutdown()
    {
        LOG("Shutting down application");
        m_Running = false;
    }

}