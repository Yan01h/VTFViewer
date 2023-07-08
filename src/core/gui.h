// gui.h

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#pragma once

class VTFFile;

class GUI
{
public:
    static void Init();
    static void Shutdown();

    static void Render();
private:
    GUI() = delete;
    ~GUI() = delete;

    static void ResetZoom(float width, float height);

    static void CenteredText(const char* label, ...);
private:
    static float _zoom;

    static unsigned int _currentMipmap;
};