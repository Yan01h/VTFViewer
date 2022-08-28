// Utility.cpp
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
 */

#include "Utility.h"

#include "MainWindow.h"

#include <string>

#include <Windows.h>

namespace VTFViewer {

    void GLFWDropCallback(GLFWwindow* window, int count, const char** paths)
    {
        if (!FileHasExtension((char*)paths[0], "vtf"))
            MessageBoxA(NULL, "This file is not a .vtf file", "", MB_OK | MB_ICONINFORMATION);
        else
            g_VTFFile.Open((char*)paths[0]);
    }

    void GLFWErrorCallback(int error, const char* description)
    {
        LOG("[GLFW] Error %i: %s", error, description);
    }

    bool FileHasExtension(char* file, const char* extension)
    {
        std::string fileString(file);
        if (fileString.substr(fileString.find_last_of(".") + 1) == extension)
            return true;
        else
            return false;
    }

    // Source from: https://stackoverflow.com/questions/8991192/check-the-file-size-without-opening-file-in-c
    int GetFileSizeFromPath(char* path)
    {
        HANDLE hFile = CreateFileA(path, GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            return -1; // error condition, could call GetLastError to find out more

        LARGE_INTEGER size;
        if (!GetFileSizeEx(hFile, &size))
        {
            CloseHandle(hFile);
            return -1; // error condition, could call GetLastError to find out more
        }

        CloseHandle(hFile);
        return size.QuadPart / 1024;
    }

    const char* GetValveImageFormatString(unsigned int format)
    {
        switch (format)
        {
        case -1:
            return "IMAGE_FORMAT_NONE";
        case 0:
            return "IMAGE_FORMAT_RGBA8888";
        case 1:
            return "IMAGE_FORMAT_ABGR8888";
        case 2:
            return "IMAGE_FORMAT_RGB888";
        case 3:
            return "IMAGE_FORMAT_BGR888";
        case 4:
            return "IMAGE_FORMAT_RGB565";
        case 5:
            return "IMAGE_FORMAT_I8";
        case 6:
            return "IMAGE_FORMAT_IA88";
        case 7:
            return "IMAGE_FORMAT_P8";
        case 8:
            return "IMAGE_FORMAT_A8";
        case 9:
            return "IMAGE_FORMAT_RGB888_BLUESCREEN";
        case 10:
            return "IMAGE_FORMAT_BGR888_BLUESCREEN";
        case 11:
            return "IMAGE_FORMAT_ARGB8888";
        case 12:
            return "IMAGE_FORMAT_BGRA8888";
        case 13:
            return "IMAGE_FORMAT_DXT1";
        case 14:
            return "IMAGE_FORMAT_DXT3";
        case 15:
            return "IMAGE_FORMAT_DXT5";
        case 16:
            return "IMAGE_FORMAT_BGRX8888";
        case 17:
            return "IMAGE_FORMAT_BGR565";
        case 18:
            return "IMAGE_FORMAT_BGRX5551";
        case 19:
            return "IMAGE_FORMAT_BGRA4444";
        case 20:
            return "IMAGE_FORMAT_DXT1_ONEBITALPHA";
        case 21:
            return "IMAGE_FORMAT_BGRA5551";
        case 22:
            return "IMAGE_FORMAT_UV88";
        case 23:
            return "IMAGE_FORMAT_UVWQ8888";
        case 24:
            return "IMAGE_FORMAT_RGBA16161616F";
        case 25:
            return "IMAGE_FORMAT_RGBA16161616";
        case 26:
            return "IMAGE_FORMAT_UVLX8888";
        }
        return "0";
    }

    std::string GetFileNameFromPath(char* path)
    {
        std::string filename(path);
        return filename.substr(filename.find_last_of("\\") + 1);
    }

}