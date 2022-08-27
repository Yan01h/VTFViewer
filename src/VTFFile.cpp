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
#include "valve/VTF.h"

#include <string>

#include <Windows.h>

namespace VTFViewer {

    VTFFile::VTFFile()
        :m_File()
    {
        LOG("VTFFile constructor");

        m_FileSize = 0;

        m_FileName = "0";

        ZeroMemory(&m_VTFHeader, sizeof(Valve::VTFHEADER));
    }

    VTFFile::~VTFFile()
    {
        LOG("VTFFile deconstructor");
        if (m_File.is_open())
            m_File.close();
    }

    void VTFFile::Open(char* path)
    {
        LOG("Opening file: %s", path);

        m_File.open(path, std::ifstream::ate | std::ifstream::binary);
        if (m_File.is_open())
        {
            LOG("File opened!");
            m_FileSize = GetFileSizeFromPath(path);

            m_FileName = GetFileNameFromPath(path);
        }
    }
    
}