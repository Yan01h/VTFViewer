// window.h

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#pragma once

class Window
{
public:
    static void Init(const char* title, int width, int height);
    static void Shutdown();

    static void Clear();
    static void PollInput();
    static void SwapBuffers();

    static const bool GetShouldClose();
private:
    Window() = delete;
    ~Window() = delete;

    static void* GetGLFWWindow();
private:
    static struct GLFWwindow* _glfwWindow;
private:
    friend class GUI;
};