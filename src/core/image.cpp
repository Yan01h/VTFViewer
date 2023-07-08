// image.cpp

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#include "image.h"

#include <glfw/glfw3.h>

Image::Image(unsigned int width, unsigned int height, char* imageData)
    :_id(0), _width(width), _height(height)
{
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}

Image::~Image()
{
    glDeleteTextures(1, &_id);
}

void* Image::GetImGuiImage() const
{
    return (void*)_id;
}

const unsigned int Image::GetWidth() const
{
    return _width;
}

const unsigned int Image::GetHeight() const
{
    return _height;
}