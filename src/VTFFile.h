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

#include <string>
#include <vector>

#include <VTFLib/VTFLib.h>

namespace VTFViewer {

    class VTFFile final
    {
    public:
        VTFFile();
        ~VTFFile();

        void OpenFromDisk(char* path);
        void Close();

        const char* GetFileName() const { return m_FileName.c_str(); }

        const float* GetReflectivity() const { return m_Reflectivity; }
        const unsigned int GetTextureID() const { return m_TextureID; }
        const std::vector<const char*>& GetFlagStrings() const { return m_FlagStrings; }
        const VTFLib::CVTFFile& GetVTFData() const { return m_VTFFile; }
    private:
        float m_Reflectivity[3];
        unsigned int m_TextureID;

		std::string m_FileName;
		std::vector<const char*> m_FlagStrings;
		VTFLib::CVTFFile m_VTFFile;
    };

}