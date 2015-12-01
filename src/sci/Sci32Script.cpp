//
//  Sci32Script.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Sci32Script.hpp"
#include "tools/Endians.hpp"

#include <iostream>

using namespace scissum;

struct Sci32Script::Header {
    uint32_t bytecodeOffset;
    uint32_t stringsOffset;
    uint32_t unknownOffset0;
    uint16_t localsCount;
    uint16_t unknown0;
    uint16_t unknown1;
    uint16_t unknown2;
    uint16_t exportsCount;
} __attribute__ ((packed));

Sci32Script::Sci32Script(uint8_t *data, size_t length)
: d_data(data), d_length(length)
{
    Header const *header = reinterpret_cast<Header const *>(data);
    
    std::cout << "Bytecode offset: " << header->bytecodeOffset << ".\n";
    std::cout << "Bytecode length: " << (header->stringsOffset - header->bytecodeOffset) << ".\n";
    
    d_bytecodeData = d_data + letoh(header->bytecodeOffset);
    d_bytecodeLength = letoh(header->stringsOffset) - letoh(header->bytecodeOffset);
    
    std::cout << "Bytecode data: " << d_bytecodeData << ".\n";
}

Sci32Script::~Sci32Script()
{
    if (d_data != nullptr) {
        delete [] d_data;
    }
}

uint8_t const *Sci32Script::data() const
{
    return d_data;
}

uint8_t *Sci32Script::data()
{
    return d_data;
}

size_t Sci32Script::length() const
{
    return d_length;
}

uint8_t const *Sci32Script::bytecodeArea() const
{
    return d_bytecodeData;
}

size_t Sci32Script::bytecodeLength() const
{
    return d_bytecodeLength;
}
