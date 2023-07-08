// application.cpp

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#include "application.h"

#include "gui.h"
#include "vtf.h"
#include "window.h"

#include "debug/debug.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

Application* Application::_instance = nullptr;

Application::Application()
{
    VTFV_ASSERT(!_instance, "An instance of Application already exists!");
    _instance = this;

    _vtfFileList.clear();

    Window::Init("VTFViewer", WINDOW_WIDTH, WINDOW_HEIGHT);
    GUI::Init();
}

Application::~Application()
{
    GUI::Shutdown();
    Window::Shutdown();
}

int Application::Run()
{
    while (!Window::GetShouldClose())
    {
        Window::PollInput();

        Window::Clear();
        GUI::Render();
        Window::SwapBuffers();
    }

    return 0;
}

std::vector<std::unique_ptr<VTFFile>>& Application::GetVTFFileList()
{
    return _vtfFileList;
}

Application& Application::Get()
{
    return *_instance;
}