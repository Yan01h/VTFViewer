// image.h

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#pragma once

class Image
{
public:
    Image(unsigned int width, unsigned int height, char* imageData);
    ~Image();

    void* GetImGuiImage() const;

    const unsigned int GetWidth() const;
    const unsigned int GetHeight() const;
private:
    Image() = delete;
private:
    unsigned int _id;
    unsigned int _width;
    unsigned int _height;
};