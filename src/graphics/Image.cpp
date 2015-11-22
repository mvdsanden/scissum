//
//  Image.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Image.hpp"
#include "PixelBuffer.hpp"

using namespace scissum;

Image::Image(size_t width, size_t height, std::unique_ptr<PixelBuffer> &&pixelBuffer)
: d_width(width), d_height(height), d_pixelBuffer(std::move(pixelBuffer))
{
}

Image::Image(size_t width, size_t height)
: d_width(width), d_height(height)
{
}

Image::~Image()
{
}

void Image::setPixelBuffer(std::unique_ptr<PixelBuffer> &&pixelBuffer)
{
    d_pixelBuffer = std::move(pixelBuffer);
}

PixelBuffer &Image::pixelBuffer()
{
    return *d_pixelBuffer.get();
}

PixelBuffer const &Image::pixelBuffer() const
{
    return *d_pixelBuffer.get();
}

size_t Image::width() const
{
    return d_width;
}

size_t Image::height() const
{
    return d_height;
}
