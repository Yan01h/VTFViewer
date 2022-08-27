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

namespace VTFViewer {

    class VTFFile final
    {
    public:
        VTFFile();
        ~VTFFile();

        
        const char* GetFileName() const { return m_FileName; }

        const Valve::VTFHEADER& GetVTFHeader() const { return m_VTFHeader; }
    private:
        char* m_FileName;
        
        std::ifstream m_File;
        Valve::VTFHEADER m_VTFHeader;
    };

}