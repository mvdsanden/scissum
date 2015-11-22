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
