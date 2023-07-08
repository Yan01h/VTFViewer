// vtf.cpp

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#include "vtf.h"

#include "image.h"

#include "debug/debug.h"

VTFFile::VTFFile()
    :_open(false)
{}

VTFFile::~VTFFile()
{
    VTFV_LOG_INFO("VTFFile '%s' destroyed", _fileName.c_str());
}

void VTFFile::OpenFromDisk(const char* filePath)
{
    Load(filePath);

    _fileName = GetFileNameFromPath(filePath);

    LoadImages();
    StoreFlagStrings();

    _open = true;
}

bool& VTFFile::GetOpen()
{
    return _open;
}

const std::string_view VTFFile::GetFileName() const
{
    return _fileName;
}

const std::vector<const char*> VTFFile::GetFlagStrings() const
{
    return _flagStrings;
}

const std::unique_ptr<Image>& VTFFile::GetThumbnail() const
{
    return _thumbnail;
}

const std::unique_ptr<Image>& VTFFile::GetTexture(unsigned int index) const
{
    if (index < 0 || index > _textures.size())
        return nullptr;
    return _textures[index];
}

void VTFFile::LoadImages()
{
    // Thumbnail
    vlUInt thumbnailWidth = GetThumbnailWidth();
    vlUInt thumbnailHeight = GetThumbnailHeight();

    vlByte* thumbnailData = new vlByte[VTFLib::CVTFFile::ComputeImageSize(thumbnailWidth, thumbnailHeight, 1,
        IMAGE_FORMAT_RGBA8888)];
    ConvertToRGBA8888(GetThumbnailData(), thumbnailData, thumbnailWidth, thumbnailHeight, GetThumbnailFormat());
    _thumbnail = std::make_unique<Image>(thumbnailWidth, thumbnailHeight, (char*)thumbnailData);
    delete[] thumbnailData;

    // Textures
    _textures.reserve(GetMipmapCount());
    
    for (vlUInt mipmap = 0; mipmap < GetMipmapCount(); mipmap++)
    {
        vlUInt width;
        vlUInt height;
        vlUInt depth;
        ComputeMipmapDimensions(GetWidth(), GetHeight(), GetDepth(), mipmap, width, height, depth);

        vlByte* imageData = new vlByte[VTFLib::CVTFFile::ComputeImageSize(width, height, depth,
            IMAGE_FORMAT_RGBA8888)];

        ConvertToRGBA8888(GetData(0, 0, 0, mipmap), imageData, width, height, GetFormat());
        _textures.push_back(std::make_unique<Image>(width, height, (char*)imageData));

        delete[] imageData;
    }
}

void VTFFile::StoreFlagStrings()
{
    unsigned int flags = GetFlags();
    _flagStrings.clear();
#pragma region flagCheck
    if (flags & TEXTUREFLAGS_POINTSAMPLE)
        _flagStrings.push_back("POINTSAMPLE");
    if (flags & TEXTUREFLAGS_TRILINEAR)
        _flagStrings.push_back("TRILINEAR");
    if (flags & TEXTUREFLAGS_CLAMPS)
        _flagStrings.push_back("CLAMPS");
    if (flags & TEXTUREFLAGS_CLAMPT)
        _flagStrings.push_back("CLAMPT");
    if (flags & TEXTUREFLAGS_ANISOTROPIC)
        _flagStrings.push_back("ANISOTROPIC");
    if (flags & TEXTUREFLAGS_HINT_DXT5)
        _flagStrings.push_back("HINT_DXT5");
    if (flags & TEXTUREFLAGS_SRGB)
        _flagStrings.push_back("SRGB");
    if (flags & TEXTUREFLAGS_NORMAL)
        _flagStrings.push_back("NORMAL");
    if (flags & TEXTUREFLAGS_NOMIP)
        _flagStrings.push_back("NOMIP");
    if (flags & TEXTUREFLAGS_NOLOD)
        _flagStrings.push_back("NOLOD");
    if (flags & TEXTUREFLAGS_MINMIP)
        _flagStrings.push_back("MINMIP");
    if (flags & TEXTUREFLAGS_PROCEDURAL)
        _flagStrings.push_back("PROCEDURAL");
    if (flags & TEXTUREFLAGS_ONEBITALPHA)
        _flagStrings.push_back("ONEBITALPHA");
    if (flags & TEXTUREFLAGS_EIGHTBITALPHA)
        _flagStrings.push_back("EIGHTBITALPHA");
    if (flags & TEXTUREFLAGS_ENVMAP)
        _flagStrings.push_back("ENVMAP");
    if (flags & TEXTUREFLAGS_RENDERTARGET)
        _flagStrings.push_back("RENDERTARGET");
    if (flags & TEXTUREFLAGS_DEPTHRENDERTARGET)
        _flagStrings.push_back("DEPTHRENDERTARGET");
    if (flags & TEXTUREFLAGS_NODEBUGOVERRIDE)
        _flagStrings.push_back("NODEBUGOVERRIDE");
    if (flags & TEXTUREFLAGS_SINGLECOPY)
        _flagStrings.push_back("SIGNLECOPY");
    if (flags & TEXTUREFLAGS_UNUSED0)
        _flagStrings.push_back("UNUSED0 (DEPRECATED)");
    if (flags & TEXTUREFLAGS_UNUSED1)
        _flagStrings.push_back("UNUSED1 (DEPRECATED)");
    if (flags & TEXTUREFLAGS_UNUSED2)
        _flagStrings.push_back("UNUSED2 (DEPRECATED)");
    if (flags & TEXTUREFLAGS_UNUSED3)
        _flagStrings.push_back("UNUSED3 (DEPRECATED)");
    if (flags & TEXTUREFLAGS_NODEPTHBUFFER)
        _flagStrings.push_back("NODEPTHBUFFER");
    if (flags & TEXTUREFLAGS_UNUSED4)
        _flagStrings.push_back("UNUSED4 (DEPRECATED)");
    if (flags & TEXTUREFLAGS_CLAMPU)
        _flagStrings.push_back("CLAMPU");
    if (flags & TEXTUREFLAGS_VERTEXTEXTURE)
        _flagStrings.push_back("VERTEXTEXTURE");
    if (flags & TEXTUREFLAGS_SSBUMP)
        _flagStrings.push_back("SSBUMP");
    if (flags & TEXTUREFLAGS_UNUSED5)
        _flagStrings.push_back("UNUSED5 (DEPRECATED)");
    if (flags & TEXTUREFLAGS_BORDER)
        _flagStrings.push_back("BORDER");
    if (flags & TEXTUREFLAGS_DEPRECATED_SPECVAR_RED)
        _flagStrings.push_back("SPECVAR_RED (DEPRECATED)");
    if (flags & TEXTUREFLAGS_DEPRECATED_SPECVAR_ALPHA)
        _flagStrings.push_back("SPECVAR_ALPHA (DEPRECATED)");
#pragma endregion
}

std::string VTFFile::GetFileNameFromPath(const char* filePath)
{
    std::string fileName(filePath);
    return fileName.substr(fileName.find_last_of("\\") + 1);
}