//
//  Palette.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Palette.hpp"

#include <cstring>

using namespace scissum;

Palette::Palette()
{
    memset(d_table, 0, sizeof(d_table));
}

Palette::~Palette()
{
    
}

Color &Palette::operator[](uint8_t index)
{
    return d_table[index];
}

Color const &Palette::operator[](uint8_t index) const
{
    return d_table[index];
}

Color *Palette::ptr()
{
    return d_table;
}

Color const *Palette::ptr() const
{
    return d_table;
}

void Palette::setSafetyPalette()
{
    memset(d_table, 0, sizeof(d_table));
    
    size_t index = 1;
    
    d_table[0].a = 255;
    
    for (size_t i = 1; i < 20; ++i) {
        d_table[i].a = 255;
        d_table[i].r = i * 12;
        d_table[i].g = i * 12;
        d_table[i].b = i * 12;
        d_table[255-i].a = 255;
        d_table[255-i].r = i * 12;
        d_table[255-i].g = i * 12;
        d_table[255-i].b = i * 12;
    }
    
    for (size_t r = 0; r < 256; r += 51) {
        for (size_t g = 0; g < 256; g += 51) {
            for (size_t b = 0; b < 256; b += 51) {
                d_table[index].a = 255;
                d_table[index].r = r;
                d_table[index].g = g;
                d_table[index].b = b;
            }
        }
    }
    
    d_table[255].a = 255;
    d_table[255].r = 255;
    d_table[255].g = 255;
    d_table[255].b = 255;    
}
