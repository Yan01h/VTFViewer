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

namespace VTFViewer {

    class Application final
    {
    public:
        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(Application&) = delete;

        void Run(int argc, char* argv[]);
        void Shutdown();
    private:
        bool m_Running;

        MainWindow* m_MainWindow;
    };

}