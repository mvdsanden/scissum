//
//  Sci32PictureLoader.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32PictureLoader.hpp"
#include "Sci32Picture.hpp"
#include "Sci32PictureCel.hpp"
#include "graphics/Palette.hpp"
#include "graphics/Image.hpp"
#include "graphics/PixelBuffer.hpp"
#include "io/Reader.hpp"
#include "tools/Endians.hpp"

#include <iostream>

using namespace scissum;

struct Sci32PictureLoader::ResourceHeader {
    uint16_t headerSize;
    uint8_t celCount; // 16bit (add next unknown)?
    uint8_t unknown0;
    uint16_t celHeaderSize; // SIZE OF CEL HEADER!
    uint32_t paletteOffset;
    uint16_t width;
    uint16_t height;
} __attribute__ ((packed));

struct Sci32PictureLoader::CelHeader {
    uint16_t width;
    uint16_t height;
    int16_t horzDisplacement;
    int16_t vertDisplacement;
    uint8_t transparentColor;
    uint8_t compressed;
    uint8_t unknown0[14];
    uint32_t pixelDataOffset; // there be pixels here.
    uint32_t literalOffset;
    uint32_t unknown1;
    uint16_t priority;
    uint16_t relativeXPos;
    uint16_t relativeYPos;
} __attribute__ ((packed));

struct Sci32PictureLoader::PaletteHeader {
    uint8_t unknown0[25];
    uint8_t startingColor;
    uint8_t unknown1[3];
    uint16_t colorCount;
    uint8_t unknown2;
    uint8_t paletteFormat; // 0=>4 bytes per entry, 1=>3 bytes per entry.
    uint8_t unknown3[5];
} __attribute__ ((packed));

void Sci32PictureLoader::readResourceHeader(std::shared_ptr<Reader> const &reader, ResourceHeader &header)
{
    size_t len = reader->read(reinterpret_cast<uint8_t*>(&header), sizeof(header));
    
    if (len != sizeof(header)) {
        throw std::runtime_error("resource seems corrupted, could not read resource header");
    }

    header.headerSize = letoh(header.headerSize);
    header.celHeaderSize = letoh(header.celHeaderSize);
    header.paletteOffset = letoh(header.paletteOffset);
    header.width = letoh(header.width);
    header.height = letoh(header.height);
    
    if (header.headerSize < sizeof(ResourceHeader)) {
        throw std::runtime_error("header size to small");
    }
    
    // Skip the rest of the resource header.
    len = reader->skip(header.headerSize - sizeof(header));
    
    if (len != header.headerSize - sizeof(header)) {
        throw std::runtime_error("resource seems corrupted, could not seek to end of cel header");
    }
}

void Sci32PictureLoader::readCelHeader(std::shared_ptr<Reader> const &reader, CelHeader &header, size_t length)
{
    size_t len = reader->read(reinterpret_cast<uint8_t*>(&header), sizeof(CelHeader));
    
    if (len != sizeof(CelHeader)) {
        throw std::runtime_error("resource seems corrupted, could not read cel header");
    }
    
    header.width = letoh(header.width);
    header.height = letoh(header.height);
    header.pixelDataOffset = letoh(header.pixelDataOffset);
    header.literalOffset = letoh(header.literalOffset);
    header.priority = letoh(header.priority);
    header.relativeXPos = letoh(header.relativeXPos);
    header.relativeYPos = letoh(header.relativeYPos);
    
    len = reader->skip(length - sizeof(CelHeader));
    
    if (len != length - sizeof(CelHeader)) {
        throw std::runtime_error("resource seems corrupted, could not skip to end of cel header");
    }
}

void Sci32PictureLoader::readPalette(std::shared_ptr<Reader> const &reader, Palette &palette)
{
    // Read the palette header.
    PaletteHeader paletteHeader;
    
    size_t len = reader->read(reinterpret_cast<uint8_t*>(&paletteHeader), sizeof(paletteHeader));
    
    if (len != sizeof(paletteHeader)) {
        throw std::runtime_error("resource seems corrupt, could not read palette header");
    }
    
    paletteHeader.colorCount = letoh(paletteHeader.colorCount);
    
    // Read the palette data.
    if (paletteHeader.paletteFormat == 0) {
        uint8_t entry[4];
        for (size_t i = paletteHeader.startingColor; i < paletteHeader.startingColor + paletteHeader.colorCount; ++i) {
            len = reader->read(reinterpret_cast<uint8_t*>(entry), sizeof(entry));
            
            if (len != sizeof(entry)) {
                throw std::runtime_error("resource seems corrupt, can't read palette data");
            }
            
            palette[i].a = entry[3];
            palette[i].r = entry[0];
            palette[i].g = entry[1];
            palette[i].b = entry[2];
        }
    } else {
        uint8_t entry[3];
        for (size_t i = paletteHeader.startingColor; i < paletteHeader.startingColor + paletteHeader.colorCount; ++i) {
            len = reader->read(reinterpret_cast<uint8_t*>(entry), sizeof(entry));
            
            if (len != sizeof(entry)) {
                throw std::runtime_error("resource seems corrupt, can't read palette data");
            }
            
            palette[i].a = 255;
            palette[i].r = entry[0];
            palette[i].g = entry[1];
            palette[i].b = entry[2];
        }
    }
}

// TODO: split up in multiple methods.
std::shared_ptr<Sci32Picture> Sci32PictureLoader::loadPicture(std::shared_ptr<Reader> const &reader)
{

    std::shared_ptr<Sci32Picture> result;
    
    size_t len = 0;
    
    ResourceHeader resourceHeader;
    
    readResourceHeader(reader, resourceHeader);
    
    result = std::make_shared<Sci32Picture>(resourceHeader.width, resourceHeader.height);
    
    if (resourceHeader.celHeaderSize < sizeof(CelHeader)) {
        throw std::runtime_error("cel header size to small");
    }
    
    // Read all cel headers.
    CelHeader celHeaders[resourceHeader.celCount];
    for (size_t i = 0; i < resourceHeader.celCount; ++i) {
        readCelHeader(reader, celHeaders[i], resourceHeader.celHeaderSize);
    }
    
    if (resourceHeader.paletteOffset < reader->readOffset()) {
        throw std::runtime_error("palette data before cel headers");
    }
    
    // Skip to the palette header.
    len = reader->skip(resourceHeader.paletteOffset - reader->readOffset());
    
    Palette palette;
    
    readPalette(reader, palette);
    
    // Loop through every cel.
    for (size_t c = 0; c < resourceHeader.celCount; ++c) {
        CelHeader &celHeader = celHeaders[c];
        
        if (reader->readOffset() > celHeader.pixelDataOffset) {
            throw std::runtime_error("resource seems corrupt, pixel data offset past current offset");
        }
        
        // Create the cel object.
        auto cel = std::unique_ptr<Sci32PictureCel>(new Sci32PictureCel(celHeader.width,
                                                                        celHeader.height,
                                                                        celHeader.horzDisplacement,
                                                                        celHeader.vertDisplacement,
                                                                        celHeader.priority,
                                                                        celHeader.relativeXPos,
                                                                        celHeader.relativeYPos));
        
        // Skip to the cels pixel data.
        len = reader->skip(celHeader.pixelDataOffset - reader->readOffset());
        
        auto image
        = std::unique_ptr<Image>(new Image(celHeader.width,
                                           celHeader.height,
                                           std::unique_ptr<PixelBuffer>(new MemoryPixelBuffer(celHeader.width * celHeader.height))));
        
        
        
        {
            
            ScopedMapReadWrite<PixelBuffer> map(image->pixelBuffer());
            
            Color *colors = map.ptr();
            
            uint8_t buffer[128];
            size_t pixelCount = celHeader.width*celHeader.height;
            while (pixelCount > 0) {
                size_t count = std::min(pixelCount, sizeof(buffer));
                
                len = reader->read(buffer, count);
                
                if (len != count) {
                    throw std::runtime_error("picture resource doesn't seem to have enough pixel data");
                }
                
                for (size_t i = 0; i < len; ++i) {
                    
                    if (buffer[i] == celHeader.transparentColor) {
                        colors->a = 0;
                        colors->r = 0;
                        colors->g = 0;
                        colors->b = 0;
                        ++colors;
                    } else {
                        *(colors++) = palette[buffer[i]];
                    }
                }
                
                pixelCount -= len;
            }
        }
        
        
        
        cel->setImage(std::move(image));
        
        result->appendCel(std::move(cel));
    }
    
    return result;
}
