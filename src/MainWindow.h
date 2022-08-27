// MainWindow.h
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
*/

#pragma once

#include "Utility.h"
#include "VTFFile.h"

#include <GLFW/glfw3.h>

namespace VTFViewer {

    extern VTFFile g_VTFFile;

    class MainWindow final
    {
    public:
        MainWindow(const char* title, uint width, uint height);
        ~MainWindow();

        void Render();

        inline void PollInput() { glfwPollEvents(); }
        inline int ShouldClose() { return glfwWindowShouldClose(m_Window); }
    private:
        MainWindow();
    private:
        GLFWwindow* m_Window;
    };

}