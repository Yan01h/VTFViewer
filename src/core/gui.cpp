// gui.cpp

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#include "gui.h"

#include <algorithm>
#include <memory>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl2.h>

#include "application.h"
#include "image.h"
#include "open_file_dialog.h"
#include "vtf.h"
#include "window.h"

#include "debug/debug.h"

#include "fonts/segoeui.h"

float GUI::_zoom = 1.0f;

unsigned int GUI::_currentMipmap = 0;

void GUI::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.LogFilename = NULL;

    ImFontConfig fontConfig;
    fontConfig.PixelSnapH = true;
    fontConfig.OversampleH = 3;
    fontConfig.OversampleV = 3;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(fonts::segoeuiData, 20.f, &fontConfig);

    GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetGLFWWindow());
    VTFV_ASSERT(window, "Window needs to be initialized before GUI");
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 10.f;
    style.WindowBorderSize = 0.f;
    style.WindowRounding = 0.f;

    style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_Header] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_Tab] = style.Colors[ImGuiCol_MenuBarBg];
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
}

void GUI::Shutdown()
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::Render()
{
    static std::vector<std::unique_ptr<VTFFile>>& vtfFileList = Application::Get().GetVTFFileList();

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Gui
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize, ImGuiCond_Always);
    ImGui::Begin("##vtf-viewer", nullptr,
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar
    );

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open"))
            {
                std::string path;

                bool ret = OpenFileDialog(path);
                if (ret)
                {
                    size_t index = vtfFileList.size();
                    vtfFileList.push_back(std::make_unique<VTFFile>());
                    vtfFileList[index]->OpenFromDisk(path.c_str());
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (vtfFileList.size() > 0)
    {
        if (ImGui::BeginTabBar("##files", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs))
        {
            static unsigned int lastImage = -1;
            static unsigned int lastMap = -2;
            for (unsigned int it = 0; it < vtfFileList.size(); it++)
            {
                std::unique_ptr<VTFFile>& vtfFile = vtfFileList[it];
                if (!vtfFile->GetOpen())
                {
                    vtfFileList.erase(vtfFileList.begin() + it);
                    lastImage = -1;

                    // End frame early, otherwise it would crash due auto-select selecting a new tab but not
                    // updating the current index
                    ImGui::EndTabBar();
                    ImGui::End();
                    ImGui::Render();
                    return;

                }

                if (ImGui::BeginTabItem(vtfFile->GetFileName().data(), &vtfFile->GetOpen()))
                {
                    ImGui::Text("File: %s (%iKB)", vtfFile->GetFileName().data(),
                        vtfFile->GetSize() / 1024);
                    ImGui::Text("Size: %ix%i", vtfFile->GetWidth(), vtfFile->GetHeight());

                    if (ImGui::CollapsingHeader("Properties"))
                    {
                        ImGui::BeginGroup();
                        ImGui::BulletText("Version: %i.%i", vtfFile->GetMajorVersion(), vtfFile->GetMinorVersion());
                        ImGui::BulletText("Frames: %i", vtfFile->GetFrameCount());
                        ImGui::BulletText("Faces: %i", vtfFile->GetFaceCount());
                        ImGui::BulletText("First Frame: %i", vtfFile->GetStartFrame());
                        float x, y, z;
                        vtfFile->GetReflectivity(x, y, z);
                        ImGui::BulletText("Reflectivity: X: %f Y: %f Z: %f", x, y, z);
                        ImGui::BulletText("Bumpmap Scale: %f", vtfFile->GetBumpmapScale());
                        ImGui::EndGroup();

                        ImGui::SameLine();

                        ImGui::BeginGroup();
                        SVTFImageFormatInfo imageInfo = VTFLib::CVTFFile::GetImageFormatInfo(vtfFile->GetFormat());
                        ImGui::BulletText("Image Format: %s (%i)", imageInfo.lpName, vtfFile->GetFormat());
                        ImGui::BulletText("Mipmaps: %i", vtfFile->GetMipmapCount());
                        SVTFImageFormatInfo thumbnailFormat = VTFLib::CVTFFile::GetImageFormatInfo(
                            vtfFile->GetThumbnailFormat());
                        ImGui::BulletText("Thumbnail Format: %s (%i)", thumbnailFormat.lpName,
                            vtfFile->GetThumbnailFormat());
                        ImGui::BulletText("Thumbnail Size: %ix%i", vtfFile->GetThumbnailWidth(),
                            vtfFile->GetThumbnailHeight());
                        ImGui::BulletText("Depth: %i", vtfFile->GetDepth());
                        ImGui::BulletText("Resources: %i", vtfFile->GetResourceCount());
                        ImGui::EndGroup();

                        ImGui::SameLine();

                        ImGui::BeginGroup();
                        ImGui::BulletText("Flags:");

                        // TODO: Remove hard-coded child height
                        float childSizeY = 5 * ImGui::CalcTextSize("TEXT").y + 4 * ImGui::GetStyle().ItemSpacing.y;
                        ImGui::BeginChild("##flags", ImVec2(0, childSizeY), true,
                            ImGuiWindowFlags_HorizontalScrollbar);
                        for (auto flag : vtfFile->GetFlagStrings())
                        {
                            ImGui::Text("- %s", flag);
                        }
                        ImGui::EndChild();

                        ImGui::EndGroup();
                    }

                    ImGui::Separator();

                    // Calculate main child width
                    float mainChildWidth = (3.f / 4.f) * ImGui::GetMainViewport()->Size.x -
                        ImGui::GetStyle().ItemSpacing.x - ImGui::GetStyle().WindowPadding.x;
                    
                    ImVec2 mainChildSize = { mainChildWidth, 0 };
                    ImGui::BeginChild("##mainImage", mainChildSize, true, ImGuiWindowFlags_HorizontalScrollbar);

                    // Calculate first zoom
                    ImVec2 windowSize = ImGui::GetWindowSize();

                    if (lastImage != it)
                    {
                        ResetZoom((float)vtfFile->GetWidth(), (float)vtfFile->GetHeight());
                        _currentMipmap = 0;
                        lastImage = it;
                    }

                    const std::unique_ptr<Image>& image = _currentMipmap == -1 ?
                        vtfFile->GetThumbnail() : vtfFile->GetTexture(_currentMipmap);

                    if (lastMap != _currentMipmap)
                    {
                        ResetZoom((float)image->GetWidth(), (float)image->GetHeight());
                        lastMap = _currentMipmap;
                    }

                    // Handle mouse wheel for zoom
                    ImGuiIO& io = ImGui::GetIO();
                    if (io.KeyCtrl)
                    {
                        if (io.MouseWheel > 0.f)
                            _zoom += 0.25f;
                        else if (io.MouseWheel < 0.f)
                            _zoom -= 0.25;
                        _zoom = std::clamp(_zoom, 0.25f, 1000.f);
                    }

                    // Display Image
                    ImVec2 imageSize = { (float)image->GetWidth() * _zoom, (float)image->GetHeight() * _zoom };
                    ImVec2 imagePos = { windowSize.x * 0.5f - imageSize.x * 0.5f,
                        windowSize.y * 0.5f - imageSize.y * 0.5f };
                    
                    // Reset image position to 0 if the image is bigger than the child due to the way
                    // imgui scrollbars work
                    if (imageSize.x > ImGui::GetContentRegionAvail().x)
                        imagePos.x = 0.f;
                    if (imageSize.y > ImGui::GetContentRegionAvail().y)
                        imagePos.y = 0.f;

                    ImGui::SetCursorPos(imagePos);
                    ImGui::Image(image->GetImGuiImage(), imageSize);

                    ImGui::EndChild();

                    ImGui::SameLine();

                    // Image selection
                    ImGui::BeginChild("##imageSelection", ImVec2(0, 0), true);

                    float buttonSizeX = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().WindowPadding.x;
                    float buttonSizeY = (buttonSizeX / vtfFile->GetWidth()) * vtfFile->GetHeight();

                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

                    if (ImGui::ImageButton(vtfFile->GetTexture(0)->GetImGuiImage(),
                        ImVec2(buttonSizeX, buttonSizeY)))
                        _currentMipmap = 0;
                    CenteredText("Full Image");

                    if (ImGui::ImageButton(vtfFile->GetThumbnail()->GetImGuiImage(),
                        ImVec2(buttonSizeX, buttonSizeY)))
                        _currentMipmap = -1;            
                    CenteredText("Thumbnail");

                    for (unsigned int it = 1; it < vtfFile->GetMipmapCount(); it++)
                    {
                        if (ImGui::ImageButton(vtfFile->GetTexture(it)->GetImGuiImage(),
                            ImVec2(buttonSizeX, buttonSizeY)))
                            _currentMipmap = it;
                        CenteredText("Mipmap %i", it);
                    }

                    ImGui::PopStyleVar();

                    ImGui::EndChild();

                    ImGui::EndTabItem();
                }
            }
        }
        ImGui::EndTabBar();
    }
    else
    {
        ImVec2 contentAvail = ImGui::GetMainViewport()->WorkSize;
        ImVec2 textSize = ImGui::CalcTextSize("Drop .vtf files here");
        ImGui::SetCursorPos(ImVec2(contentAvail.x / 2 - textSize.x / 2, contentAvail.y / 2 - textSize.y / 2));
        ImGui::TextDisabled("Drop .vtf files here");
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void GUI::ResetZoom(float width, float height)
{
    ImVec2 windowSize = ImGui::GetWindowSize();

    ImVec2 requiredZoom = { windowSize.x / width, windowSize.y / height };
    _zoom = std::min(requiredZoom.x, requiredZoom.y);
}

void GUI::CenteredText(const char* label, ...)
{
    char buffer [16];
    va_list args;
    va_start(args, label);
    vsprintf(buffer, label, args);
    va_end(args);

    ImVec2 textSize = ImGui::CalcTextSize(buffer);
    float cursorPosX = ImGui::GetWindowSize().x * 0.5f - textSize.x * 0.5f;
    
    ImGui::SetCursorPosX(cursorPosX);
    ImGui::Text(buffer);
}