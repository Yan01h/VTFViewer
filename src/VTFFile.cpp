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

    void VTFFile::Close()
    {
        LOG("Closing current vtf file");
        if (m_File.is_open())
            m_File.close();

        m_FileSize = 0;

        m_FileName = "0";

        ZeroMemory(&m_VTFHeader, sizeof(Valve::VTFHEADER));
    }

    void VTFFile::Open(char* path)
    {
        LOG("Opening file: %s", path);

        m_File.open(path, std::ifstream::binary);
        if (m_File.is_open())
        {
            LOG("File opened!");
            m_FileSize = GetFileSizeFromPath(path);

            m_FileName = GetFileNameFromPath(path);

            m_File.read((char*)&m_VTFHeader, sizeof(Valve::VTFHEADER));
            // Version checking (Only files with version 7.3 are allowed at the moment
            if (m_VTFHeader.version[0] != 7 || m_VTFHeader.version[1] != 3)
            {
                LOG("Attempted to open vtf file version %i.%i", m_VTFHeader.version[0],
                    m_VTFHeader.version[1]);
                Close();
                MessageBoxA(NULL, "Only vtf file version 7.3 is allowed at the moment",
                    "Error with vtf version", MB_OK | MB_ICONERROR);
            }
        }
    }
    
}