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
#include <VTFLib/VTFLib.h>

namespace VTFViewer {

    Application* Application::s_App = nullptr;

    Application::Application()
        :m_CurrentFileName(), m_CurrentTextureID(0), m_MainWindow(nullptr), m_Running(true)
    {
        LOG("Application constructor");
        s_App = this;

        m_CurrentFlags.reserve(TEXTUREFLAGS_COUNT);

        m_MainWindow = new MainWindow("VTF Viewer", 300, 500);

        SetupTexture(&m_CurrentTextureID);
    }

    Application::~Application()
    {
        LOG("Application deconstructor");
        CloseFile();
        delete m_MainWindow;
    }

    void Application::CloseFile()
    {
        if(m_CurrentVTFFile.IsLoaded())
            m_CurrentVTFFile.Destroy();
        m_CurrentFlags.clear();
        m_CurrentFlags.reserve(TEXTUREFLAGS_COUNT);
    }

    void Application::LoadFile(char* path)
    {
        CloseFile();
        m_CurrentVTFFile.Load(path, false);
        m_CurrentFileName = GetFileNameFromPath(path);
        m_CurrentFlags = GetValveVTFFlagString(m_CurrentVTFFile.GetFlags());

        vlByte * dest = new vlByte[VTFLib::CVTFFile::ComputeImageSize(m_CurrentVTFFile.GetWidth(), 
            m_CurrentVTFFile.GetHeight(), 1, IMAGE_FORMAT_RGBA8888)];
        VTFLib::CVTFFile::ConvertToRGBA8888(m_CurrentVTFFile.GetData(0, 0, 0, 0), dest,
            m_CurrentVTFFile.GetWidth(), m_CurrentVTFFile.GetHeight(), m_CurrentVTFFile.GetFormat());
        LoadTexture(dest, m_CurrentVTFFile.GetWidth(), m_CurrentVTFFile.GetHeight());
        delete[] dest;
    }

    void Application::Run()
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