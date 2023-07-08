// window.cpp

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#define GLFW_EXPOSE_NATIVE_WIN32
#define WIN32_LEAN_AND_MEAN

#include "window.h"

#include <glfw/glfw3.h>

#include "application.h"
#include "vtf.h"

#include "debug/debug.h"

GLFWwindow* Window::_glfwWindow = nullptr;

void DropCallback(GLFWwindow* window, int count, const char** paths);
void ErrorCallback(int errorCode, const char* description);

void Window::Init(const char* title, int width, int height)
{
    glfwSetErrorCallback(ErrorCallback);

    int ret = glfwInit();
    VTFV_ASSERT(ret == GLFW_TRUE, "Failed to init glfw!");

    _glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    VTFV_ASSERT(_glfwWindow, "Failed to create glfw window!");
    glfwSetDropCallback(_glfwWindow, DropCallback);
    glfwMakeContextCurrent(_glfwWindow);
    glfwSwapInterval(1);

    glClearColor(0.f, 0.f, 0.f, 1.f);
}

void Window::Shutdown()
{
    glfwDestroyWindow(_glfwWindow);
    glfwTerminate();
}

void Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::PollInput()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(_glfwWindow);
}

const bool Window::GetShouldClose()
{
    return glfwWindowShouldClose(_glfwWindow);
}

void* Window::GetGLFWWindow()
{
    return _glfwWindow;
}

void ErrorCallback(int errorCode, const char* description)
{
    VTFV_LOG_ERROR("[GLFW] %s (Code: %i)", description, errorCode);
}

void DropCallback(GLFWwindow* window, int count, const char** paths)
{
    std::vector<std::unique_ptr<VTFFile>>& vtfFileList = Application::Get().GetVTFFileList();

    for (unsigned int it = 0; it < count; it++)
    {
        size_t index = vtfFileList.size();
        vtfFileList.push_back(std::make_unique<VTFFile>());
        vtfFileList[index]->OpenFromDisk(paths[it]);
    }
}