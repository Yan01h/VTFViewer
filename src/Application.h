// Application.h
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
*/

#pragma once

#include "MainWindow.h"

#include <string>
#include <vector>

#include <VTFLib/VTFLib.h>

namespace VTFViewer {

    class Application final
    {
    public:
        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(Application&) = delete;

        void CloseFile();
        void LoadFile(char* path);
        void Run();
        void Shutdown();

        const VTFLib::CVTFFile& GetCurrentFile() const { return m_CurrentVTFFile; }
        const char* GetCurrentFileName() const { return m_CurrentFileName.c_str(); }
        const std::vector<const char*>& GetCurrentFlagStrings() const { return m_CurrentFlags; }
        const unsigned int GetCurrentTextureID() const { return m_CurrentTextureID; }
    public:
        static Application* Get() { return s_App; }
    private:
        std::string m_CurrentFileName;
        std::vector<const char*> m_CurrentFlags;
        unsigned int m_CurrentTextureID;
        VTFLib::CVTFFile m_CurrentVTFFile;
        MainWindow* m_MainWindow;
        bool m_Running;
    private:
        static Application* s_App;
    };

}