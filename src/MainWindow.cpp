// MainWindow.cpp
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
*/

#include "MainWindow.h"

#include "Utility.h"
#include "VTFFile.h"
#include "fonts/Segoeui.h"
#include "valve/VTF.h"

#include <assert.h>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl2.h>
#include <NFD/nfd.h>

namespace VTFViewer {

    VTFFile g_VTFFile;

    MainWindow::MainWindow(const char* title, uint width, uint height)
        :m_Window(nullptr)
    {
        LOG("MainWindow constructor");
        glfwSetErrorCallback(GLFWErrorCallback);
        assert(glfwInit() && "Failed to initialize GLFW!");

        m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
        assert(m_Window && "Failed to create GLFW window!");

        glfwMakeContextCurrent(m_Window);
        glfwSetDropCallback(m_Window, GLFWDropCallback);
        glfwSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;
        io.LogFilename = NULL;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(Fonts::g_SegoeuiData, 19.f);

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL2_Init();

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowBorderSize = 0.f;
    }

    MainWindow::~MainWindow()
    {
        LOG("MainWindow destructor");
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void MainWindow::Render()
    {
        const Valve::VTFHEADER& vtfHeader = g_VTFFile.GetVTFHeader();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiStyle& style = ImGui::GetStyle();

        int menuBarHeight = 0;
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open"))
                {
                    nfdchar_t* path = NULL;
                    nfdresult_t result = NFD_OpenDialog("vtf", NULL, &path);
                    if (result == NFD_ERROR)
                        MessageBoxA(NULL, "An error has occurred!", "Error", MB_OK | MB_ICONERROR);
                    if (!FileHasExtension(path, "vtf"))
                        MessageBoxA(NULL, "This file is not a .vtf file", "", MB_OK | MB_ICONINFORMATION);
                    else
                        g_VTFFile.Open(path);
                }
                if (ImGui::MenuItem("Close"))
                {
                    g_VTFFile.Close();
                }
                ImGui::EndMenu();
            }
            menuBarHeight = ImGui::GetWindowSize().y;
            ImGui::EndMainMenuBar();
        }

        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);
        ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
        ImGui::SetNextWindowSize(ImVec2(width, height - menuBarHeight));
        ImGui::Begin("Test", (bool*)false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("File name: %s (%iKB)", g_VTFFile.GetFileName(), g_VTFFile.GetFileSize());
        ImGui::Text("Size: %ix%i", vtfHeader.width, vtfHeader.height);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, style.WindowPadding.y));
        ImGui::Separator();
        ImVec2 childSize(width - 2 * style.WindowPadding.x,
            height - ImGui::GetCursorPosY() - menuBarHeight - style.WindowPadding.y);
        ImGui::BeginChild("Image", childSize, true);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 0.3f));
        ImVec2 cursorPos(childSize.x / 2 - ImGui::CalcTextSize("Drag .vtf file here").x / 2,
            childSize.y / 2 - ImGui::CalcTextSize("Drag .vtf file here").y / 2);
        ImGui::SetCursorPos(cursorPos);
        ImGui::Text("Drag .vtf file here");
        ImGui::PopStyleColor();

        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_Window);
    }

}