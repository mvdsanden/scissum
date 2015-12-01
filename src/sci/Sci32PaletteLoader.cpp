//
//  Sci32PaletteLoader.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32PaletteLoader.hpp"
#include "io/Reader.hpp"
#include "tools/Endians.hpp"
#include "graphics/Palette.hpp"

#include <stdexcept>

using namespace scissum;

struct Sci32PaletteLoader::PaletteHeader {
    uint8_t unknown0[25];
    uint8_t startingColor;
    uint8_t unknown1[3];
    uint16_t colorCount;
    uint8_t unknown2;
    uint8_t paletteFormat; // 0=>4 bytes per entry, 1=>3 bytes per entry.
    uint8_t unknown3[5];
} __attribute__ ((packed));

void Sci32PaletteLoader::readPalette(std::shared_ptr<Reader> const &reader, Palette &palette)
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

void Sci32PaletteLoader::load(std::shared_ptr<Reader> const &reader, Palette &palette)
{
    readPalette(reader, palette);
}
