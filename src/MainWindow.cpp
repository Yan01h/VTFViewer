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

#include "Application.h"
#include "Utility.h"
#include "VTFFile.h"
#include "fonts/Segoeui.h"

#include <vector>

#include <assert.h>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl2.h>
#include <NFD/nfd.h>
#include <VTFLib/VTFLib.h>

namespace VTFViewer {

    MainWindow::MainWindow(const char* title, unsigned int width, unsigned int height)
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
        style.Colors[ImGuiCol_Header] = ImVec4(0.f, 0.f, 0.f, 0.f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2f, 0.2f, 0.2f, 0.94f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.94f);
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
        static VTFFile& vtfFile = Application::Get()->GetCurrentVTFFile();
        static const VTFLib::CVTFFile& vtfData = vtfFile.GetVTFData();

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
                    if (result != NFD_CANCEL)
                    {
                        if (!FileHasExtension(path, "vtf"))
                            MessageBoxA(NULL, "This file is not a .vtf file", "", MB_OK | MB_ICONINFORMATION);
                        else
                            vtfFile.OpenFromDisk(path);
                    }
                }
                if (ImGui::MenuItem("Close"))
                {
                    vtfFile.Close();
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
        ImGui::Begin("VTFViewer", (bool*)false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("File name: %s (%iKB)", vtfFile.GetFileName(), vtfData.GetSize() / 1024);
        ImGui::Text("Size: %ix%i", vtfData.GetWidth(), vtfData.GetHeight());

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        if (ImGui::CollapsingHeader("Properties"))
        {
            ImGui::Separator();
            ImGui::BulletText("Version: %i.%i", vtfData.GetMajorVersion(), vtfData.GetMinorVersion());
            ImGui::BulletText("Width: %ipx", vtfData.GetWidth());
            ImGui::BulletText("Height: %ipx", vtfData.GetHeight());
            ImGui::BulletText("Frames: %i", vtfData.GetFrameCount());
            ImGui::BulletText("Faces: %i", vtfData.GetFaceCount());
            ImGui::BulletText("First Frame: %i", vtfData.GetStartFrame());
            ImGui::BulletText("Reflectivity: [%f | %f | %f]", vtfFile.GetReflectivity()[0],
                vtfFile.GetReflectivity()[1], vtfFile.GetReflectivity()[2]);
            ImGui::BulletText("Bumpmap Scale: %f", vtfData.GetBumpmapScale());
            ImGui::BulletText("Image Format: %s (%i)",
                vtfData.IsLoaded() ? vtfData.GetImageFormatInfo(vtfData.GetFormat()).lpName : "",
                vtfData.GetFormat());
            ImGui::BulletText("Mipmaps: %i", vtfData.GetMipmapCount());
            ImGui::BulletText("Thumbnail Image Format: %s (%i)",
                vtfData.IsLoaded() ? vtfData.GetImageFormatInfo(vtfData.GetThumbnailFormat()).lpName : "",
                vtfData.GetThumbnailFormat());
            ImGui::BulletText("Thumbnail Image Width: %ipx", vtfData.GetThumbnailWidth());
            ImGui::BulletText("Thumbnail Image Height: %ipx", vtfData.GetThumbnailHeight());
            if (vtfData.GetMinorVersion() >= 2)
                ImGui::BulletText("Depth: %ipx", vtfData.GetDepth());
            if (vtfData.GetMinorVersion() >= 3)
                ImGui::BulletText("Resources: %i", vtfData.GetResourceCount());
            if (ImGui::TreeNode("Flags"))
            {
                for (const char* flag : vtfFile.GetFlagStrings())
                    ImGui::Text("%s", flag);
                ImGui::TreePop();
            }
        }
        ImGui::PopStyleVar();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, style.WindowPadding.y));
        ImGui::Separator();
        ImVec2 childSize(width - 2 * style.WindowPadding.x,
            height - ImGui::GetCursorPosY() - menuBarHeight - style.WindowPadding.y);
        ImGui::BeginChild("Image", childSize, true);

        if (!vtfData.IsLoaded())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 0.3f));
            ImVec2 cursorPos(childSize.x / 2 - ImGui::CalcTextSize("Drag .vtf file here").x / 2,
                childSize.y / 2 - ImGui::CalcTextSize("Drag .vtf file here").y / 2);
            ImGui::SetCursorPos(cursorPos);
            ImGui::Text("Drag .vtf file here");
            ImGui::PopStyleColor();
        }
        else
        {
            ImGui::Image((void*)(intptr_t)vtfFile.GetTextureID(),
                ImVec2(vtfData.GetWidth(), vtfData.GetHeight()));
        }

        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_Window);
    }

}