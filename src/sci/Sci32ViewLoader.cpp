//
//  Sci32ViewLoader.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32ViewLoader.hpp"
#include "Sci32PaletteLoader.hpp"
#include "Sci32View.hpp"
#include "Sci32ViewCel.hpp"
#include "io/Reader.hpp"
#include "tools/Endians.hpp"
#include "graphics/PixelBuffer.hpp"
#include "graphics/Palette.hpp"
#include "graphics/Image.hpp"

#include <stdexcept>
#include <algorithm>
#include <iostream>

#define SCISSUM_SCI32_LE_CONVERT(x) (x = letoh(x))

using namespace scissum;

struct Sci32ViewLoader::ResourceHeader
{
    uint16_t headerSize;
    uint8_t groupCount;
    uint8_t flags;
    uint16_t version;
    uint16_t unknown0;
    uint32_t paletteOffset;
    uint8_t groupSize;
    uint8_t celSize;
    uint16_t width;
    uint16_t height;
    uint16_t unknown1;
} __attribute__ ((packed));

struct Sci32ViewLoader::GroupHeader
{
    uint16_t seekEntry;
    uint8_t celCount;
    uint32_t unknown0;
    uint8_t unknown1;
    uint32_t unknown2;
    uint32_t celDataOffset;
} __attribute__ ((packed));

struct Sci32ViewLoader::CelHeader
{
    uint16_t width;
    uint16_t height;
    int16_t horzDisplacement;
    int16_t vertDisplacement;
    uint8_t transparentColor;
    uint8_t compressed;
    uint16_t unknown0;
    uint32_t unknown1;
    uint32_t rleDataSize;
    uint16_t unknown2;
    uint16_t unknown3;
    uint32_t rleDataOffset;
    uint32_t literalDataOffset;
    uint32_t scanlineTableOffset;
    uint32_t resourceSize; // ?
    uint32_t unknown4;
    uint32_t unknown5;
    uint32_t unknown6;
} __attribute__ ((packed));

void Sci32ViewLoader::readResourceHeader(std::shared_ptr<Reader> const &reader, ResourceHeader &header)
{
    size_t len = reader->read(reinterpret_cast<uint8_t*>(&header), sizeof(header));
    
    if (len != sizeof(header)) {
        throw std::runtime_error("resource seems corrupted, could not read header");
    }
    
    SCISSUM_SCI32_LE_CONVERT(header.headerSize);
    SCISSUM_SCI32_LE_CONVERT(header.version);
    SCISSUM_SCI32_LE_CONVERT(header.paletteOffset);
    SCISSUM_SCI32_LE_CONVERT(header.width);
    SCISSUM_SCI32_LE_CONVERT(header.height);
    
    // For some reason the header size field is not inclusive with the header size value.
    header.headerSize += 2;
    
    if (header.headerSize < sizeof(header)) {
        throw std::runtime_error("header size too small");
    }
    
    reader->skip(header.headerSize - sizeof(header));
}

void Sci32ViewLoader::readGroupHeader(std::shared_ptr<Reader> const &reader, GroupHeader &header, size_t length)
{
    if (length < sizeof(header)) {
        throw std::runtime_error("header size too small");
    }
    
    size_t len = reader->read(reinterpret_cast<uint8_t*>(&header), sizeof(header));
    
    if (len != sizeof(header)) {
        throw std::runtime_error("resource seems corrupted, could not read header");
    }
    
    SCISSUM_SCI32_LE_CONVERT(header.seekEntry);
    SCISSUM_SCI32_LE_CONVERT(header.celDataOffset);
    
    reader->skip(length - sizeof(header));
}

void Sci32ViewLoader::readCelHeader(std::shared_ptr<Reader> const &reader, CelHeader &header, size_t length)
{
    if (length < sizeof(header)) {
        throw std::runtime_error("header size too small");
    }
    
    size_t len = reader->read(reinterpret_cast<uint8_t*>(&header), sizeof(header));
    
    if (len != sizeof(header)) {
        throw std::runtime_error("resource seems corrupted, could not read header");
    }
    
    SCISSUM_SCI32_LE_CONVERT(header.width);
    SCISSUM_SCI32_LE_CONVERT(header.height);
    SCISSUM_SCI32_LE_CONVERT(header.horzDisplacement);
    SCISSUM_SCI32_LE_CONVERT(header.vertDisplacement);
    SCISSUM_SCI32_LE_CONVERT(header.rleDataSize);
    SCISSUM_SCI32_LE_CONVERT(header.rleDataOffset);
    SCISSUM_SCI32_LE_CONVERT(header.literalDataOffset);
    SCISSUM_SCI32_LE_CONVERT(header.scanlineTableOffset);
    SCISSUM_SCI32_LE_CONVERT(header.resourceSize);
    
    reader->skip(length - sizeof(header));
}

void parseRunLengthEncodedPixelBuffer(std::unique_ptr<PixelBuffer> &pixelBuffer,
                                      std::shared_ptr<Reader> const &reader,
                                      size_t pixelCount, uint8_t const *rleData,
                                      size_t rleDataSize, Palette &palette)
{
 //   std::cout << "Start offset: " << reader->readOffset() << ".\n";
    
    pixelBuffer = std::unique_ptr<PixelBuffer>(new MemoryPixelBuffer(pixelCount));

    ScopedMapReadWrite<PixelBuffer> map(*pixelBuffer.get());
    
    Color *rgb = map.ptr();
    Color *rgbEnd = rgb + pixelCount;
    
    uint8_t buffer[128];
    
    for (uint8_t const *rle = rleData; rle != rleData + rleDataSize; ++rle) {
        uint8_t control = *rle & 0xC0;
        size_t length = *rle & 0x3F;
        
        //std::cout << "[" << std::hex << static_cast<size_t>(control) << "] " << std::dec << length << ".\n";
        
        switch (control) {
            case 0x1:
                length += 64;
                // FT!
            case 0x00:
                if (length > (rgbEnd - rgb)) {
                    throw std::runtime_error("resource seems corrupted, not enough space for pixel data");
                }
                
                if (reader->read(buffer, length) != length) {
                    throw std::runtime_error("resource seems corrupted, could not read enough literal data");
                }
                
                for (uint8_t *i = buffer; i != buffer + length; ++i) {
                    *(rgb++) = palette[*i];
                }
                break;
                
            case 0x80:
                if (length > (rgbEnd - rgb)) {
                    throw std::runtime_error("resource seems corrupted, not enough space for pixel data");
                }
                
                uint8_t b;
                if (reader->read(&b, 1) != 1) {
                    throw std::runtime_error("resource seems corrupted, could not read enough literal data");
                }
                
                for (size_t i = 0; i < length; ++i) {
                    *(rgb++) = palette[b];
                }
                break;
                
            case 0xC0:
                if (length > (rgbEnd - rgb)) {
                    throw std::runtime_error("resource seems corrupted, not enough space for pixel data");
                }
                
                memset(rgb, 0, sizeof(Color)*length);
                rgb += length;
                break;
        }
    }
}

void parseRunLengthEncodedImage(std::unique_ptr<Image> &image,
                                std::shared_ptr<Reader> const &reader,
                                size_t width, size_t height,
                                uint8_t const *rleData, size_t rleDataSize, size_t literalDataOffset,
                                Palette &palette)
{
    if (reader->readOffset() > literalDataOffset) {
        throw std::runtime_error("already read past literal data");
    }
    
    reader->skip(literalDataOffset - reader->readOffset());
    
    std::unique_ptr<PixelBuffer> pixelBuffer;
    
    parseRunLengthEncodedPixelBuffer(pixelBuffer,
                                     reader,
                                     width * height,
                                     rleData, rleDataSize,
                                     palette);
    
    assert(pixelBuffer.get());
    
    image = std::unique_ptr<Image>(new Image(width, height, std::move(pixelBuffer)));
}

void parseBitmapImage(std::unique_ptr<Image> &image,
                      std::shared_ptr<Reader> const &reader,
                      size_t width, size_t height,
                      size_t literalDataOffset, Palette &palette)
{
    if (reader->readOffset() > literalDataOffset) {
        throw std::runtime_error("already read past literal data");
    }
    
    auto pixelBuffer = std::unique_ptr<PixelBuffer>(new MemoryPixelBuffer(width * height));
    
    {
        ScopedMapReadWrite<PixelBuffer> map(*pixelBuffer.get());
        
        Color *rgb = map.ptr();
        
        uint8_t buffer[128];
        
        size_t pixelCount = width * height;
        
        while (pixelCount != 0) {
            size_t count = std::min(pixelCount, sizeof(buffer));
            
            size_t len = reader->read(buffer, count);
            
            if (len != count) {
                throw std::runtime_error("resource seems corrupt, could not read enough pixel data");
            }
            
            for (uint8_t *j = buffer; j != buffer + count; ++j) {
                *(rgb++) = palette[*j];
            }
            
            pixelCount -= count;
        }
    }
    
    image = std::unique_ptr<Image>(new Image(width, height, std::move(pixelBuffer)));
}

void parseImage(std::unique_ptr<Image> &image,
                                    std::shared_ptr<Reader> const &reader,
                                    size_t width, size_t height,
                                    uint8_t const *rleData, size_t rleDataSize, size_t literalDataOffset,
                                    Palette &palette)
{
    if (!rleData) {
        parseBitmapImage(image, reader, width, height, literalDataOffset, palette);
    } else {
        parseRunLengthEncodedImage(image, reader, width, height, rleData, rleDataSize, literalDataOffset, palette);
    }
}

std::shared_ptr<Sci32View> Sci32ViewLoader::load(std::shared_ptr<Reader> const &reader, Palette &palette)
{
    ResourceHeader resourceHeader;
    readResourceHeader(reader, resourceHeader);
    
    size_t totalCelCount = 0;

    // Read all group headers.
    GroupHeader groupHeaders[resourceHeader.groupCount];
    for (size_t i = 0; i < resourceHeader.groupCount; ++i) {
        readGroupHeader(reader, groupHeaders[i], resourceHeader.groupSize);
        totalCelCount += groupHeaders[i].celCount;
    }
    
    CelHeader celHeaders[totalCelCount];
    size_t celIndex = 0;
    
    // Read all cel headers.
    for (size_t groupI = 0; groupI < resourceHeader.groupCount; ++groupI) {
        GroupHeader &groupHeader = groupHeaders[groupI];
        
        if (reader->readOffset() > groupHeader.celDataOffset) {
            throw std::runtime_error("already read past cel data");
        }
        
        reader->skip(groupHeader.celDataOffset - reader->readOffset());
        
        for (size_t celI = 0; celI < groupHeader.celCount; ++celI) {
            readCelHeader(reader, celHeaders[celIndex++], resourceHeader.celSize);
        }
    }
    
    if (resourceHeader.paletteOffset != 0) {
        Sci32PaletteLoader paletteLoader;
        reader->skip(resourceHeader.paletteOffset - reader->readOffset());
        paletteLoader.load(reader, palette);
    }

    uint8_t *rleData[totalCelCount];
    memset(rleData, 0, sizeof(uint8_t*) * totalCelCount);

    try {
        
        // Read all cel rle data.
        celIndex = 0;
        for (size_t groupI = 0; groupI < resourceHeader.groupCount; ++groupI) {
            GroupHeader &groupHeader = groupHeaders[groupI];
            
            for (size_t celI = 0; celI < groupHeader.celCount; ++celI, ++celIndex) {
                CelHeader &celHeader = celHeaders[celIndex];
                
                if (celHeader.rleDataOffset == 0) {
                    continue;
                }
                
                // Savety check.
                if (celHeader.rleDataSize > 10*1024*1024) {
                    throw std::runtime_error("rle data to big");
                }
                
                uint8_t *data = rleData[celIndex] = new uint8_t [ celHeader.rleDataSize ];
                
                reader->skip(celHeader.rleDataOffset - reader->readOffset());
                
                if (reader->read(data, celHeader.rleDataSize) != celHeader.rleDataSize) {
                    throw std::runtime_error("resource seems corrupt, could not read all rle data");
                }
            }
            
        }
        
        std::vector<Sci32View::Group> groups(resourceHeader.groupCount);
        
        // Read all cel images and create the cels and group stuctures.
        celIndex = 0;
        for (size_t groupI = 0; groupI < resourceHeader.groupCount; ++groupI) {
            GroupHeader &groupHeader = groupHeaders[groupI];
            
            Sci32View::Group group(groupHeader.celCount);
            
            for (size_t celI = 0; celI < groupHeader.celCount; ++celI, ++celIndex) {
                CelHeader &celHeader = celHeaders[celIndex];
                
                auto cel = std::unique_ptr<Sci32ViewCel>(new Sci32ViewCel(celHeader.width, celHeader.height,
                                                                          celHeader.horzDisplacement, celHeader.vertDisplacement));
                
                
                std::unique_ptr<Image> image;
                
                parseImage(image,
                           reader,
                           celHeader.width, celHeader.height,
                           rleData[celIndex],
                           celHeader.rleDataSize,
                           celHeader.literalDataOffset,
                           palette);
                
                cel->setImage(std::move(image));
                
                cel->image();
                
                group[celI] = std::move(cel);
            }
            
            groups[groupI] = std::move(group);
        }
   
        return std::move(std::unique_ptr<Sci32View>(new Sci32View(resourceHeader.width, resourceHeader.height, std::move(groups))));
        
    } catch (...) {
        for (size_t i = 0; i < totalCelCount; ++i) {
            if (rleData[i]) {
                delete [] rleData[i];
                rleData[i] = nullptr;
            }
        }
        throw;
    }
}