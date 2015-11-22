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

using namespace scissum;

// TODO: split up in multiple methods.
std::shared_ptr<Sci32Picture> Sci32PictureLoader::loadPicture(std::shared_ptr<Reader> const &reader)
{

    std::shared_ptr<Sci32Picture> result;
    
    struct ResourceHeader {
        uint16_t headerSize;
        uint8_t celCount; // 16bit (add next unknown)?
        uint8_t unknown0;
        uint16_t celHeaderSize; // SIZE OF CEL HEADER!
        uint32_t paletteOffset;
        uint16_t width;
        uint16_t height;
    } __attribute__ ((packed));
    
    struct CelHeader {
        uint16_t width;
        uint16_t height;
        uint16_t horzDisplacement;
        uint16_t vertDisplacement;
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
    
    struct PaletteHeader {
        uint8_t unknown0[25];
        uint8_t startingColor;
        uint8_t unknown1[3];
        uint16_t colorCount;
        uint8_t unknown2;
        uint8_t paletteFormat; // 0=>4 bytes per entry, 1=>3 bytes per entry.
        uint8_t unknown3[5];
    } __attribute__ ((packed));
    
    size_t offset = 0;
    size_t len = 0;
    
    ResourceHeader resourceHeader;
    
    len = reader->read(reinterpret_cast<uint8_t*>(&resourceHeader), sizeof(resourceHeader));
    
    if (len < sizeof(resourceHeader)) {
        throw std::runtime_error("resource seems corrupted, can't read resource header");
    }
    
    offset += len;
    
    if (resourceHeader.headerSize < sizeof(ResourceHeader)) {
        throw std::runtime_error("header size to small");
    }
    
    result = std::make_shared<Sci32Picture>(resourceHeader.width, resourceHeader.height);
    
    // Skip the rest of the resource header.
    len = reader->skip(resourceHeader.headerSize - sizeof(resourceHeader));
    
    if (len != resourceHeader.headerSize - sizeof(resourceHeader)) {
        throw std::runtime_error("resource seems corrupted, can't seek to cel header");
    }
    
    offset += len;
    
    if (resourceHeader.celCount > 32) {
        throw std::runtime_error("too many cells in picture");
    }
    
    if (resourceHeader.celHeaderSize < sizeof(CelHeader)) {
        throw std::runtime_error("cel header size to small");
    }
    
    // Read all cel headers.
    CelHeader celHeaders[resourceHeader.celCount];
    for (size_t i = 0; i < resourceHeader.celCount; ++i) {
        len = reader->read(reinterpret_cast<uint8_t*>(celHeaders + i), sizeof(CelHeader));
        
        if (len != sizeof(CelHeader)) {
            throw std::runtime_error("resource seems corrupted, can't read cel header");
        }
        
        offset += len;
        
        len = reader->skip(resourceHeader.celHeaderSize - sizeof(CelHeader));
        
        if (len != resourceHeader.celHeaderSize - sizeof(CelHeader)) {
            throw std::runtime_error("resource seems corrupted, can't skip to next cel header");
        }
        
        offset += len;
    }
    
    if (resourceHeader.paletteOffset < offset) {
        throw std::runtime_error("palette data before cel headers");
    }
    
    // Skip to the palette header.
    len = reader->skip(resourceHeader.paletteOffset - offset);
    
    if (len != resourceHeader.paletteOffset - offset) {
        throw std::runtime_error("resource seems corrupt, can't seek to palette");
    }
    
    offset += len;
    
    // Read the palette header.
    PaletteHeader paletteHeader;
    
    len = reader->read(reinterpret_cast<uint8_t*>(&paletteHeader), sizeof(paletteHeader));
    
    if (len != sizeof(paletteHeader)) {
        throw std::runtime_error("resource seems corrupt, can't read palette header");
    }
    
    offset += len;
    
    Palette palette;
    
    // Read the palette data.
    if (paletteHeader.paletteFormat == 0) {
        uint8_t entry[4];
        for (size_t i = paletteHeader.startingColor; i < paletteHeader.startingColor + paletteHeader.colorCount; ++i) {
            len = reader->read(reinterpret_cast<uint8_t*>(entry), sizeof(entry));
            
            if (len != sizeof(entry)) {
                throw std::runtime_error("resource seems corrupt, can't read palette data");
            }
            
            offset += len;
            
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
            
            offset += len;

            palette[i].a = 255;
            palette[i].r = entry[0];
            palette[i].g = entry[1];
            palette[i].b = entry[2];
        }
    }
    
    // Loop through every cel.
    for (size_t c = 0; c < resourceHeader.celCount; ++c) {
        CelHeader &celHeader = celHeaders[c];
        
        if (offset > celHeader.pixelDataOffset) {
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
        len = reader->skip(celHeader.pixelDataOffset - offset);
        
        if (len != celHeader.pixelDataOffset - offset) {
            throw std::runtime_error("resource seems corrupt, can't seek to pixel data");
        }
        
        offset += len;
        
        auto image = std::unique_ptr<Image>(new Image(celHeader.width,
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
                
                offset += len;
                
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
