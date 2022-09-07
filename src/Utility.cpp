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

#include "Application.h"
#include "MainWindow.h"

#include <string>
#include <vector>

#include <Windows.h>

namespace VTFViewer {

    std::string GetFileNameFromPath(char* path)
    {
        std::string filename(path);
        return filename.substr(filename.find_last_of("\\") + 1);
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

    std::vector<const char*>& GetValveVTFFlagString(unsigned int flags)
    {
        static std::vector<const char*> flagNames;
        flagNames.clear();
#pragma region flagCheck
        if (flags & TEXTUREFLAGS_POINTSAMPLE)
            flagNames.push_back("POINTSAMPLE");
        if (flags & TEXTUREFLAGS_TRILINEAR)
            flagNames.push_back("TRILINEAR");
        if (flags & TEXTUREFLAGS_CLAMPS)
            flagNames.push_back("CLAMPS");
        if (flags & TEXTUREFLAGS_CLAMPT)
            flagNames.push_back("CLAMPT");
        if (flags & TEXTUREFLAGS_ANISOTROPIC)
            flagNames.push_back("ANISOTROPIC");
        if (flags & TEXTUREFLAGS_HINT_DXT5)
            flagNames.push_back("HINT_DXT5");
        if (flags & TEXTUREFLAGS_SRGB)
            flagNames.push_back("SRGB");
        if (flags & TEXTUREFLAGS_NORMAL)
            flagNames.push_back("NORMAL");
        if (flags & TEXTUREFLAGS_NOMIP)
            flagNames.push_back("NOMIP");
        if (flags & TEXTUREFLAGS_NOLOD)
            flagNames.push_back("NOLOD");
        if (flags & TEXTUREFLAGS_MINMIP)
            flagNames.push_back("MINMIP");
        if (flags & TEXTUREFLAGS_PROCEDURAL)
            flagNames.push_back("PROCEDURAL");
        if (flags & TEXTUREFLAGS_ONEBITALPHA)
            flagNames.push_back("ONEBITALPHA");
        if (flags & TEXTUREFLAGS_EIGHTBITALPHA)
            flagNames.push_back("EIGHTBITALPHA");
        if (flags & TEXTUREFLAGS_ENVMAP)
            flagNames.push_back("ENVMAP");
        if (flags & TEXTUREFLAGS_RENDERTARGET)
            flagNames.push_back("RENDERTARGET");
        if (flags & TEXTUREFLAGS_DEPTHRENDERTARGET)
            flagNames.push_back("DEPTHRENDERTARGET");
        if (flags & TEXTUREFLAGS_NODEBUGOVERRIDE)
            flagNames.push_back("NODEBUGOVERRIDE");
        if (flags & TEXTUREFLAGS_SINGLECOPY)
            flagNames.push_back("SIGNLECOPY");
        if (flags & TEXTUREFLAGS_UNUSED0)
            flagNames.push_back("UNUSED0 (DEPRECATED)");
        if (flags & TEXTUREFLAGS_UNUSED1)
            flagNames.push_back("UNUSED1 (DEPRECATED)");
        if (flags & TEXTUREFLAGS_UNUSED2)
            flagNames.push_back("UNUSED2 (DEPRECATED)");
        if (flags & TEXTUREFLAGS_UNUSED3)
            flagNames.push_back("UNUSED3 (DEPRECATED)");
        if (flags & TEXTUREFLAGS_NODEPTHBUFFER)
            flagNames.push_back("NODEPTHBUFFER");
        if (flags & TEXTUREFLAGS_UNUSED4)
            flagNames.push_back("UNUSED4 (DEPRECATED)");
        if (flags & TEXTUREFLAGS_CLAMPU)
            flagNames.push_back("CLAMPU");
        if (flags & TEXTUREFLAGS_VERTEXTEXTURE)
            flagNames.push_back("VERTEXTEXTURE");
        if (flags & TEXTUREFLAGS_SSBUMP)
            flagNames.push_back("SSBUMP");
        if (flags & TEXTUREFLAGS_UNUSED5)
            flagNames.push_back("UNUSED5 (DEPRECATED)");
        if (flags & TEXTUREFLAGS_BORDER)
            flagNames.push_back("BORDER");
        if (flags & TEXTUREFLAGS_DEPRECATED_SPECVAR_RED)
            flagNames.push_back("SPECVAR_RED (DEPRECATED)");
        if (flags & TEXTUREFLAGS_DEPRECATED_SPECVAR_ALPHA)
            flagNames.push_back("SPECVAR_ALPHA (DEPRECATED)");
#pragma endregion
        return flagNames;
    }

    void GLFWDropCallback(GLFWwindow* window, int count, const char** paths)
    {
        if (FileHasExtension((char*)paths[0], "vtf"))
            Application::Get()->LoadFile((char*)paths[0]);
        else
            MessageBoxA(NULL, "This file is not a .vtf file", "", MB_OK | MB_ICONINFORMATION);
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

    void LoadTexture(unsigned char* data, int width, int height)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    void SetupTexture(unsigned int* textureID)
    {
        GLuint gltexture;
        glGenTextures(1, &gltexture);
        glBindTexture(GL_TEXTURE_2D, gltexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        *textureID = gltexture;
    }

}