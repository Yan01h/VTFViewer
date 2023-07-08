// vtf.h

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <vtflib/VTFLib.h>

class Image;

class VTFFile : public VTFLib::CVTFFile
{
public:
    VTFFile();
    ~VTFFile();

    void OpenFromDisk(const char* filePath);

    bool& GetOpen();

    const std::string_view         GetFileName() const;
    const std::vector<const char*> GetFlagStrings() const;

    const std::unique_ptr<Image>& GetThumbnail() const;
    const std::unique_ptr<Image>& GetTexture(unsigned int index) const;
private:
    void LoadImages();
    void StoreFlagStrings();

    static std::string GetFileNameFromPath(const char* filePath);
private:
    bool _open;

    std::unique_ptr<Image> _thumbnail;
    std::vector<std::unique_ptr<Image>> _textures;

    std::string _fileName;

    std::vector<const char*> _flagStrings;
};