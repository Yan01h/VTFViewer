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
#include "VTFFile.h"

#include <string>
#include <vector>

namespace VTFViewer {

    class Application final
    {
    public:
        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(Application&) = delete;

        static Application* Get() { return s_App; }

        void Run();
        void Shutdown();

        VTFFile& GetCurrentVTFFile() { return m_CurrentVTFFile; }
    private:
        static Application* s_App;

        bool m_Running;

        MainWindow* m_MainWindow;
        VTFFile m_CurrentVTFFile;
    };

}