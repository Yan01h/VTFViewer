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

    std::string GetFileNameFromPath(char* path)
    {
        std::string filename(path);
        return filename.substr(filename.find_last_of("\\") + 1);
    }

}