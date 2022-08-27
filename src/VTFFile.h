// VTFFile.h
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
*/

#pragma once

#include "valve/VTF.h"

#include <iostream>
#include <fstream>
#include <string>

namespace VTFViewer {

    class VTFFile final
    {
    public:
        VTFFile();
        ~VTFFile();

        void Close();
        void Open(char* path);

        const bool IsFileOpen() const { return m_File.is_open(); }
        const char* GetFileName() const { return m_FileName.c_str(); }
        const int GetFileSize() const { return m_FileSize; }

        const Valve::VTFHEADER& GetVTFHeader() const { return m_VTFHeader; }
    private:
        int m_FileSize;

        std::ifstream m_File;
        std::string m_FileName;

        Valve::VTFHEADER m_VTFHeader;
    };

}