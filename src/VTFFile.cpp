// VTFFile.cpp
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
*/

#include "VTFFile.h"

#include "Utility.h"

namespace VTFViewer {

    VTFFile::VTFFile()
    {
        LOG("VTFFile constructor");
        m_Reflectivity[0] = 0.f;
        m_Reflectivity[1] = 0.f;
        m_Reflectivity[2] = 0.f;
        m_TextureID = 0;

        m_FileName = "";
        m_FlagStrings.reserve(TEXTUREFLAGS_COUNT);
    }

    VTFFile::~VTFFile()
    {
        LOG("VTFFile destructor");
        Close();
    }

    void VTFFile::OpenFromDisk(char* path)
    {
        Close();
        m_VTFFile.Load(path, false);
        m_VTFFile.GetReflectivity(m_Reflectivity[0], m_Reflectivity[1], m_Reflectivity[2]);
        m_FileName = GetFileNameFromPath(path);
        m_FlagStrings = GetValveVTFFlagStrings(m_VTFFile.GetFlags());

        vlByte* dest = new vlByte[VTFLib::CVTFFile::ComputeImageSize(m_VTFFile.GetWidth(),
            m_VTFFile.GetHeight(), 1, IMAGE_FORMAT_RGBA8888)];

        VTFLib::CVTFFile::ConvertToRGBA8888(m_VTFFile.GetData(0, 0, 0, 0), dest,
            m_VTFFile.GetWidth(), m_VTFFile.GetHeight(), m_VTFFile.GetFormat());

        if (!m_TextureID)
            SetupTexture(&m_TextureID);
        LoadTexture(dest, m_VTFFile.GetWidth(), m_VTFFile.GetHeight());
        delete[] dest;
    }

    void VTFFile::Close()
    {
        if (m_VTFFile.IsLoaded())
            m_VTFFile.Destroy();
        m_Reflectivity[0] = 0.f;
        m_Reflectivity[1] = 0.f;
        m_Reflectivity[2] = 0.f;
        m_FileName.clear();
        m_FlagStrings.clear();
        m_FlagStrings.reserve(TEXTUREFLAGS_COUNT);
    }

}