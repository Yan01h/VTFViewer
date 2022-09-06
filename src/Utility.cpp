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
#include "valve/VTF.h"

#include <string>
#include <vector>

#include <Windows.h>

namespace VTFViewer {

    void GLFWDropCallback(GLFWwindow* window, int count, const char** paths)
    {
        //if (!FileHasExtension((char*)paths[0], "vtf"))
        //    MessageBoxA(NULL, "This file is not a .vtf file", "", MB_OK | MB_ICONINFORMATION);
        //else
        //    g_VTFFile.Open((char*)paths[0]);
    }

    void GLFWErrorCallback(int error, const char* description)
    {
        LOG("[GLFW] Error %i: %s", error, description);
    }

    void LoadTexture(unsigned char* data, GLuint* texture, int width, int height)
    {
        GLuint gltexture;
        glGenTextures(1, &gltexture);
        glBindTexture(GL_TEXTURE_2D, gltexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        *texture = gltexture;
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

    //const char* GetValveImageFormatString(unsigned int format)
    //{
    //    switch (format)
    //    {
    //    case -1:
    //        return "IMAGE_FORMAT_NONE";
    //    case 0:
    //        return "IMAGE_FORMAT_RGBA8888";
    //    case 1:
    //        return "IMAGE_FORMAT_ABGR8888";
    //    case 2:
    //        return "IMAGE_FORMAT_RGB888";
    //    case 3:
    //        return "IMAGE_FORMAT_BGR888";
    //    case 4:
    //        return "IMAGE_FORMAT_RGB565";
    //    case 5:
    //        return "IMAGE_FORMAT_I8";
    //    case 6:
    //        return "IMAGE_FORMAT_IA88";
    //    case 7:
    //        return "IMAGE_FORMAT_P8";
    //    case 8:
    //        return "IMAGE_FORMAT_A8";
    //    case 9:
    //        return "IMAGE_FORMAT_RGB888_BLUESCREEN";
    //    case 10:
    //        return "IMAGE_FORMAT_BGR888_BLUESCREEN";
    //    case 11:
    //        return "IMAGE_FORMAT_ARGB8888";
    //    case 12:
    //        return "IMAGE_FORMAT_BGRA8888";
    //    case 13:
    //        return "IMAGE_FORMAT_DXT1";
    //    case 14:
    //        return "IMAGE_FORMAT_DXT3";
    //    case 15:
    //        return "IMAGE_FORMAT_DXT5";
    //    case 16:
    //        return "IMAGE_FORMAT_BGRX8888";
    //    case 17:
    //        return "IMAGE_FORMAT_BGR565";
    //    case 18:
    //        return "IMAGE_FORMAT_BGRX5551";
    //    case 19:
    //        return "IMAGE_FORMAT_BGRA4444";
    //    case 20:
    //        return "IMAGE_FORMAT_DXT1_ONEBITALPHA";
    //    case 21:
    //        return "IMAGE_FORMAT_BGRA5551";
    //    case 22:
    //        return "IMAGE_FORMAT_UV88";
    //    case 23:
    //        return "IMAGE_FORMAT_UVWQ8888";
    //    case 24:
    //        return "IMAGE_FORMAT_RGBA16161616F";
    //    case 25:
    //        return "IMAGE_FORMAT_RGBA16161616";
    //    case 26:
    //        return "IMAGE_FORMAT_UVLX8888";
    //    }
    //    return "0";
    //}

    std::string GetFileNameFromPath(char* path)
    {
        std::string filename(path);
        return filename.substr(filename.find_last_of("\\") + 1);
    }

    std::vector<const char*>& GetValveVTFFlagString(unsigned int flags)
    {
        static std::vector<const char*> flagNames;
        flagNames.clear();
#pragma region flagCheck
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_POINTSAMPLE)
            flagNames.push_back("TEXTUREFLAGS_POINTSAMPLE");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_TRILINEAR)
            flagNames.push_back("TEXTUREFLAGS_TRILINEAR");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_CLAMPS)
            flagNames.push_back("TEXTUREFLAGS_CLAMPS");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_CLAMPT)
            flagNames.push_back("TEXTUREFLAGS_CLAMPT");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_ANISOTROPIC)
            flagNames.push_back("TEXTUREFLAGS_ANISOTROPIC");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_HINT_DXT5)
            flagNames.push_back("TEXTUREFLAGS_HINT_DXT5");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_PWL_CORRECTED)
            flagNames.push_back("TEXTUREFLAGS_PWL_CORRECTED");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_NORMAL)
            flagNames.push_back("TEXTUREFLAGS_NORMAL");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_NOMIP)
            flagNames.push_back("TEXTUREFLAGS_NOMIP");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_NOLOD)
            flagNames.push_back("TEXTUREFLAGS_NOLOD");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_ALL_MIPS)
            flagNames.push_back("TEXTUREFLAGS_ALL_MIPS");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_PROCEDURAL)
            flagNames.push_back("TEXTUREFLAGS_PROCEDURAL");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_ONEBITALPHA)
            flagNames.push_back("TEXTUREFLAGS_ONEBITALPHA");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_EIGHTBITALPHA)
            flagNames.push_back("TEXTUREFLAGS_EIGHTBITALPHA");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_ENVMAP)
            flagNames.push_back("TEXTUREFLAGS_ENVMAP");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_RENDERTARGET)
            flagNames.push_back("TEXTUREFLAGS_RENDERTARGET");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_DEPTHRENDERTARGET)
            flagNames.push_back("TEXTUREFLAGS_DEPTHRENDERTARGET");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_NODEBUGOVERRIDE)
            flagNames.push_back("TEXTUREFLAGS_NODEBUGOVERRIDE");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_SINGLECOPY)
            flagNames.push_back("TEXTUREFLAGS_SIGNLECOPY");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_PRE_SRGB)
            flagNames.push_back("TEXTUREFLAGS_PRE_SRGB");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_NODEPTHBUFFER)
            flagNames.push_back("TEXTUREFLAGS_NODEPTHBUFFER");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_CLAMPU)
            flagNames.push_back("TEXTUREFLAGS_CLAMPU");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_VERTEXTEXTURE)
            flagNames.push_back("TEXTUREFLAGS_VERTEXTEXTURE");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_SSBUMP)
            flagNames.push_back("TEXTUREFLAGS_SSBUMP");
        if (flags & Valve::CompiledVtfFlags::TEXTUREFLAGS_BORDER)
            flagNames.push_back("TEXTUREFLAGS_BORDER");
#pragma endregion
        return flagNames;
    }

}