// application.h

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#pragma once

#include <memory>
#include <vector>

#ifdef main
#   undef main
#endif
#define main vtfvMain

class VTFFile;

class Application
{
public:
    Application();
    ~Application();

    int Run();

    std::vector<std::unique_ptr<VTFFile>>& GetVTFFileList();

    static Application& Get();
private:
    std::vector<std::unique_ptr<VTFFile>> _vtfFileList;

    static Application* _instance;
};