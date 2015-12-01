//
//  PixelBuffer.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "PixelBuffer.hpp"

#include <cstdlib>
#include <stdexcept>
#include <new>

using namespace scissum;

MemoryPixelBuffer::MemoryPixelBuffer(size_t pixelCount)
: d_size(0), d_pixels(nullptr)
{
    setSize(pixelCount);
}

MemoryPixelBuffer::~MemoryPixelBuffer()
{
    if (d_pixels != nullptr) {
        free(d_pixels);
    }
}

void MemoryPixelBuffer::setSize(size_t pixelCount)
{
    if (d_pixels != nullptr) {
        free(d_pixels);
        d_pixels = nullptr;
        d_size = 0;
    }
    
    d_size = pixelCount;
    
    // Allocate pixel memory alligned on a 16 byte boundary.
    if (posix_memalign(reinterpret_cast<void**>(&d_pixels), 16, sizeof(Color)*pixelCount) != 0) {
        throw std::bad_alloc();
    }
}

size_t MemoryPixelBuffer::size() const
{
    return d_size;
}

Color *MemoryPixelBuffer::mapReadWrite()
{
    return d_pixels;
}

Color const *MemoryPixelBuffer::mapReadOnly() const
{
    return d_pixels;
}

void MemoryPixelBuffer::unmap() const
{
    // Nothing to do.
}
